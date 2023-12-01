void clearInputBuffer()
{
    // while (kbhit())
    // {
    //     getchar(); // Read and discard one character at a time
    // }
    fflush(stdout);
}
void die(const char *s)
{
    perror(s);
    exit(1);
}
int keepRunning = 1;
struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}
void handleSigInt(int sig)
{
    if (sig == SIGINT)
    {
        printf("\nExiting loop...\n");
        keepRunning = 0; // Set the flag to exit the loop
    }
}

void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

int kbhit()
{
    struct timeval tv;
    fd_set read_fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &read_fds);
}