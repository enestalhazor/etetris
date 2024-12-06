#include "setup_stdio.h"
#include <signal.h>
#include <termios.h>
#include "enes_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int terminal_descriptor = -1;
static struct termios terminal_original;
static struct termios terminal_settings;

static void restore(void)
{
    if (terminal_descriptor != -1)
    {
        tcsetattr(terminal_descriptor, TCSANOW, &terminal_original);
    }
}

void restore_screen(void)
{
    fprintf(stdout, ESCAPE "[?25h" ESCAPE "c");
    fflush(stdout);
    restore();
}

static void handle_sigint(int signal)
{
    restore_screen();
    exit(0);
}

void *setup_stdin(void)
{
    if (terminal_descriptor != -1)
    {
        return NULL;
    }

    terminal_descriptor = STDIN_FILENO;

    if (tcgetattr(terminal_descriptor, &terminal_original) ||
        tcgetattr(terminal_descriptor, &terminal_settings))
    {
        return NULL;
    }

    if (isatty(STDIN_FILENO))
    {
        setvbuf(stdin, NULL, _IONBF, 0);
    }
    if (isatty(STDERR_FILENO))
    {
        setvbuf(stderr, NULL, _IONBF, 0);
    }

    atexit(restore);

    /* Let BREAK cause a SIGINT in input. */
    terminal_settings.c_iflag &= ~IGNBRK;
    terminal_settings.c_iflag |= BRKINT;

    /* Ignore framing and parity errors in input. */
    terminal_settings.c_iflag |= IGNPAR;
    terminal_settings.c_iflag &= ~PARMRK;

    /* Do not strip eighth bit on input. */
    terminal_settings.c_iflag &= ~ISTRIP;

    /* Do not do newline translation on input. */
    terminal_settings.c_iflag &= ~(INLCR | IGNCR | ICRNL);

    /* Use 8-bit characters. This too may affect standard streams,
     * but any sane C library can deal with 8-bit characters. */
    terminal_settings.c_cflag &= ~CSIZE;
    terminal_settings.c_cflag |= CS8;

    /* Enable receiver. */
    terminal_settings.c_cflag |= CREAD;

    /* Let INTR/QUIT/SUSP/DSUSP generate the corresponding signals. */
    terminal_settings.c_lflag |= ISIG;

    /* Enable noncanonical mode.
     * This is the most important bit, as it disables line buffering etc. */
    terminal_settings.c_lflag &= ~ICANON;

    /* Disable echoing input characters. */
    terminal_settings.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);

    /* Disable implementation-defined input processing. */
    terminal_settings.c_lflag &= ~IEXTEN;

    /* To maintain best compatibility with normal behaviour of terminals,
     * we set TIME=0 and MAX=1 in noncanonical mode. This means that
     * read() will block until at least one byte is available. */
    terminal_settings.c_cc[VTIME] = 0;
    terminal_settings.c_cc[VMIN] = 1;

    /* Set the new terminal settings.
     * Note that we don't actually check which ones were successfully
     * set and which not, because there isn't much we can do about it. */
    tcsetattr(terminal_descriptor, TCSANOW, &terminal_settings);

    printf(ESCAPE "c");
    fprintf(stdout, ESCAPE "[?25l");
    fflush(stdout);
    signal(SIGINT, handle_sigint);
}