#include <stdio.h>
#include "renderer.h"
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "tetromino.h"
#include <string.h>
#include "physics_engine.h"
#include "linked_list.h"
#include <signal.h>
#include <termios.h>
#include "enes_util.h"
#include "gameplay.h"

static int terminal_descriptor = -1;
static struct termios terminal_original;
static struct termios terminal_settings;

static void restore_screen(void)
{
    fprintf(stdout, ESCAPE "[?25h" ESCAPE "c");
    fflush(stdout);
}

static void handle_sigint(int signal)
{
    restore_screen();
    exit(0);
}

static void restore(void)
{
    if (terminal_descriptor != -1)
    {
        tcsetattr(terminal_descriptor, TCSANOW, &terminal_original);
    }
}

void *setup_stdin()
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

    fprintf(stdout, ESCAPE "[?25l");
    fflush(stdout);
    signal(SIGINT, handle_sigint);
}

int main(void)
{
    setup_stdin();
    struct scene scene = scene_create();
    struct renderer_parameters parameters;
    parameters.resolution_x = 40;
    parameters.resolution_y = 40;
    parameters.scene = &scene;
    parameters.work = &gameplay_rule;


    struct scene_object ground = scene_create_object(1000, parameters.resolution_x, 2);
    struct scene_object left_wall = scene_create_object(1001, 2, parameters.resolution_y - 2);
    struct scene_object right_wall = scene_create_object(1002, 2, parameters.resolution_y - 2);
    ground.y = parameters.resolution_y - 2;
    right_wall.x = parameters.resolution_x - 2;
    memset(ground.texture, '#', ground.width * ground.height);
    memset(left_wall.texture, '#', left_wall.width * left_wall.height);
    memset(right_wall.texture, '#', right_wall.width * right_wall.height);

    scene_add_object(&scene, ground);
    scene_add_object(&scene, left_wall);
    scene_add_object(&scene, right_wall);

    struct scene_object a = tetromino_create(4, 't');
    a.x = 21;
    a.y = 0;
    scene_add_object(&scene, a);

    
    pthread_t t1;
    pthread_create(&t1, NULL, &renderer_start, &parameters);

    pthread_t t2;
    pthread_create(&t2, NULL, &gameplay_input, NULL);

    pthread_join(t1, NULL);



    return 0;
}