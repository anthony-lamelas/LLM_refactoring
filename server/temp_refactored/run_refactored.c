#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

static int DB;

// Function to handle errors by printing the error message and terminating the program
static int error(char *who) {
    perror(who);
    kill(0, SIGKILL);
    exit(1);
}

// Function to read a specified number of bytes from a file descriptor
static int xread(int fd, char *buf, int len) {
    int remain = len;
    int total_read = 0;

    if (DB) {
        printf("xread %d:", len);
        fflush(stdout);
    }

    while (total_read != len) {
        int n = read(fd, buf + total_read, remain);
        if (n < 0) error("read");
        if (n == 0) return total_read;
        if (n <= remain) {
            remain -= n;
            total_read += n;
        }
    }
    buf[total_read] = 0;

    if (DB) {
        printf(" %s", buf);
        if (buf[total_read - 1] != '\n') printf("\n");
        fflush(stdout);
    }

    return total_read;
}

// Function to spawn a process and set up pipes for communication
static void spawn(int r[2], int w[2], char **ev, char *path, char *name, char *dict, char *opt) {
    if (pipe(r) != 0) error("pipe r");
    if (pipe(w) != 0) error("pipe w");

    pid_t pid = fork();
    if (pid == -1) error("fork");

    if (pid == 0) {
        char *args[4] = {name, dict, opt, NULL};

        if (dup2(r[0], STDIN_FILENO) < 0) error("dup2 r[0]");
        if (dup2(w[1], STDOUT_FILENO) < 0) error("dup2 w[1]");

        close(r[1]);
        close(w[0]);

        execve(path, args, ev);
        error(path);
    }

    close(r[0]);
    close(w[1]);
}

int main(int argc, char **argv, char **envp) {
    int gen_read[2], gen_write[2]; // pipes for generator read/write
    int prog_read[2], prog_write[2]; // pipes for program read/write

    static char guess[20], report[20];
    static char xyzzy[] = "xyzzy";
    char *cp, *prog = "./prog";
    int bytes_read, end = 0;
    int max_guesses = 6;

    if (argc < 2) {
        fprintf(stderr, "usage: %s dict [start [target]]\n", argv[0]);
        return 1;
    }

    DB = !!getenv("DEBUG");

    if ((cp = getenv("GUESSES")))
        max_guesses = atoi(cp);
    if ((cp = getenv("PROG")))
        prog = cp;

    spawn(gen_read, gen_write, envp, "./gen", "gen", argv[1], argc == 4 ? argv[3] : NULL);
    spawn(prog_read, prog_write, envp, prog, "prog", argv[1], NULL);

    // Wait for generator to be ready; read the prompt
    char buffer[20];
    xread(gen_write[0], buffer, 2);
    if (buffer[0] != '?') {
        xread(gen_write[0], buffer + 2, 12);
        printf("gen: %s", buffer);
        xread(gen_write[0], buffer, 2);
    }

    // Wait for program prompt
    xread(prog_write[0], buffer, 2);

    memcpy(guess, argc > 2 ? argv[2] : "blast", 5);
    guess[5] = '\n';  // mimic reading from stdin

    for (int i = 0; i < max_guesses && end != 1; ++i) {
        printf("guess%d: %s", i + 1, guess);

        bytes_read = write(gen_read[1], guess, 6);
        if (bytes_read < 0) error("write gen_read[1]");
        bytes_read = xread(gen_write[0], report, 12);
        if (bytes_read != 12) {
            if (bytes_read < 0) error("read gen_read[0]");
            printf("bad read from gen_read[0]: %d\n", bytes_read);
            return 2;
        }

        printf("gen: %s", report);

        xread(gen_write[0], guess, 2);
        if (guess[0] != '?') {
            xread(gen_write[0], guess + 2, 14);
            printf("gen result end=%d: %s", end, guess);
            if (end == 2) {
                close(gen_read[1]);
                close(prog_read[1]);
                return 0;
            }
            close(gen_write[0]);
            close(gen_read[1]);
            end = 1;
        }

        if (end == 2) {
            printf("ERROR: prog reported win, gen does not\n");
            close(gen_read[1]);
            close(prog_read[1]);
            return 1;
        }

        bytes_read = write(prog_read[1], report, bytes_read);
        if (bytes_read < 0) error("write prog_read[1]");
        bytes_read = xread(prog_write[0], guess, 6);
        if (bytes_read != 6) {
            if (bytes_read < 0) error("read prog_read");
            printf("bad read from prog_read: %d {%.*s}\n", bytes_read, bytes_read, guess);
            return 3;
        }
        if (strncmp(guess, xyzzy, sizeof(xyzzy) - 1) == 0) {
            printf("ERROR: desired word not in this dictionary\n");
            close(gen_read[1]);
            close(prog_read[1]);
            return 1;
        }

        if (xread(prog_write[0], report, 2) == 0) {
            printf("prog: EOF\n");
            if (end == 1) {
                close(gen_read[1]);
                close(prog_read[1]);
                return 0;
            }
            end = 2;
            close(prog_read[1]);
            close(prog_write[0]);
        }
    }

    return 0;
}