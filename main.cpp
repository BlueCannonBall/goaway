#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: Too few arguments" << std::endl;
        return EXIT_FAILURE;
    }

    signal(SIGCHLD, [](int) {
        waitpid(-1, nullptr, WNOHANG);
    });

    int pipe_fds[2];
    if (pipe(pipe_fds) == -1) {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    pid_t pid;
    if ((pid = fork()) == -1) {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    } else if (!pid) {
        close(pipe_fds[0]); // Close unused read end
        fcntl(pipe_fds[1], F_SETFD, FD_CLOEXEC);

        for (int i = 0; i < argc; ++i) {
            argv[i] = argv[i + 1];
        }

        freopen("/dev/null", "r", stdin);
        freopen("/dev/null", "w", stdout);
        freopen("/dev/null", "w", stderr);

        setsid();
        if (execvp(argv[0], argv) == -1) {
            int error = errno;
            write(pipe_fds[1], &error, sizeof(int));
            close(pipe_fds[1]);
            return EXIT_FAILURE;
        }
    }

    close(pipe_fds[1]); // Close unused write end

    int error;
    if (read(pipe_fds[0], &error, sizeof(int)) == sizeof(int)) {
        std::cerr << "Error: " << strerror(error) << std::endl;
        close(pipe_fds[0]);
        return EXIT_FAILURE;
    }

    close(pipe_fds[0]);
    return EXIT_SUCCESS;
}
