#!/bin/bash

# Compile the client code
gcc -o client_net client_net.c -lpthread -lrt

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Client compilation successful."

    # Run the compiled client binary in the background
    ./client_net &

    # Compile the server code
    gcc -o server_comm server_comm.c -lpthread -lrt

    # Check if compilation was successful
    if [ $? -eq 0 ]; then
        echo "Server compilation successful."

        # Run the compiled server binary
        ./server_comm

        # Clean up server binary
        rm server_comm
    else
        echo "Server compilation failed."
    fi

    # Clean up client binary
    pkill -f client_net
    rm client_net
else
    echo "Client compilation failed."
fi
