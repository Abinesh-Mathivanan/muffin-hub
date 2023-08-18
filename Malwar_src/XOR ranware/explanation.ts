/* The provided code is a asic ransomware-like program in C. It aims to encrypt files in the current directory using a simple XOR encryption scheme:

1. **Include Headers:** We include standard C library headers like `<dirent.h>`, `<fcntl.h>`, `<stdint.h>`, `<stdio.h>`, `<stdlib.h>`, `<string.h>`, `<sys/mman.h>`, and `<sys/stat.h>`.

2. **Define Constants:**
   - `RANSOMED_EXT`: This is the new extension added to the encrypted files.
   - `CHARSET`: A character set used for generating the encryption key.
   - `KEY_LEN`: The length of the encryption key.

3. **Constants and Function Definitions:**
   - `never_displayed`: A constant string indicating that a message is never displayed.
   - `rand_str`: A function that generates a random string for the encryption key using characters from the `CHARSET`.
   - `encrypt_block`: A function that performs XOR encryption on a single byte using the provided key.
   - `is_filename_proper`: A function that checks if a filename is suitable for encryption.
   - `encrypt_file`: A function that encrypts a file using XOR encryption and creates a new file with the `.beens` extension.

4. **Main Function:**
   - The main function starts by generating a random encryption key and allocating memory for it.
   - It then opens the current directory for processing using the `opendir` function.
   - It iterates through the files in the directory using the `readdir` function.
   - For each file, it calls the `encrypt_file` function to perform the encryption process if the filename is suitable.
   - Once all files are processed, the directory is closed using `closedir`.

The `encrypt_file` function performs:
   - Checks if the filename is suitable for encryption using the `is_filename_proper` function.
   - Opens the original file for reading and gets its size using the `fstat` function.
   - Generates a new filename with the `.osiris` extension and opens it for writing.
   - Allocates memory for reading the original file using `mmap`.
   - Allocates memory for the encrypted content using `alloca`.
   - Iterates through each byte of the original file, encrypts it using `encrypt_block`, and stores the result in the new memory.
   - Writes the encrypted content to the new file and removes the original file.
   - Closes both files. */