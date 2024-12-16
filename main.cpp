#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: Too few arguments" << std::endl;
        return EXIT_FAILURE;
    }

    pid_t result;
    if ((result = fork()) == -1) {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    } else if (!result) {
        for (int i = 0; i < argc; ++i) {
            argv[i] = argv[i + 1];
        }

        int stderr_fd = fcntl(STDERR_FILENO, F_DUPFD_CLOEXEC, 3); // Save stderr
        int null_file = open("/dev/null", O_RDWR);
        dup2(null_file, STDOUT_FILENO);
        dup2(null_file, STDERR_FILENO);
        dup2(null_file, STDIN_FILENO);
        close(null_file);

        if (execvp(argv[0], argv) == -1) {
            dup2(stderr_fd, STDERR_FILENO); // Restore stderr
            close(stderr_fd);

            std::cerr << "Error: " << strerror(errno) << std::endl;
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
