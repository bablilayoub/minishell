#!/bin/bash

# Get the PID of your program
pid=78215

# Function to check for file descriptor leaks
check_fd_leaks() {
    # Get the list of open files for the PID of your program and count the lines
    num_fds=$(lsof -p $pid | wc -l)
    
    # Print the number of open file descriptors
    echo "Number of open file descriptors for PID $pid: $num_fds"
}

# Main loop to continuously check for file descriptor leaks
while true; do
    check_fd_leaks
    # Sleep for a short duration before checking again
    sleep 3
done

