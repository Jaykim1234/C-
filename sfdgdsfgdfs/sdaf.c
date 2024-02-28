#include <stdio.h>

int main(int argc, char *argv[]) {
    // Check if there are enough command line arguments
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <server_ip1> <server_ip2> ... <server_ipN>\n", argv[0]);
        return 1; // Return error code
    }

    // Iterate through command line arguments starting from index 1
    for (int i = 1; i < argc; i++) {
        printf("Server %d IP Address: %s\n", i, argv[i]);
    }

    return 0;
}
