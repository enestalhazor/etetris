#include <stdio.h>
#include <string.h>
#include "enes_util.h"
#include <unistd.h>

int get_random_number(int min, int max)
{
    FILE *f = fopen("/dev/urandom", "r");

    char buffer[4];

    for (int i = 0; i < 4; i++)
    {
        buffer[i] = fgetc(f);
    }

    fclose(f);

    const int *p = (const int *)buffer;
    unsigned int value = *p;

    return value % (max - min + 1) + min;
}

int power(int x, int y)
{
    int f = 1;

    for (int a = 0; a < y; a++)
    {
        f = x * f;
    }

    return f;
}

void read_string(char *wsentence)
{
    fgets(wsentence, 500, stdin);
    wsentence[strlen(wsentence) - 1] = 0;
}

char *string(const char *s1, const char *w)
{
    for (int i = 0; i < strlen(s1); i++)
    {
        const char *current = s1 + i;

        if (strlen(current) < strlen(w))
        {
            return 0;
        }

        int counter = 0;

        for (int j = 0; j < strlen(w); j++)
        {
            if (w[j] == current[j])
            {
                counter = counter + 1;
            }
            else
            {
                break;
            }
        }

        if (counter == strlen(w))
        {
            return (char *)current;
        }
    }
    return 0;
}

int replace(char *main, const char *target, const char *replacement)
{
    // first find the substring (target).
    char *found = strstr(main, target);

    // if the substring couldn't be found then return 0.
    if (found == NULL)
    {
        return 0;
    }

    // calculate how much we want to shift.
    int n = strlen(target) - strlen(replacement);
    // create a pointer that shows the string part that we want the shift
    char *shift = found + strlen(target);

    // if n is positive shift to left n times.
    if (n > 0)
    {
        for (int i = 0; i < strlen(shift) + 1; i++)
        {
            *(shift - n + i) = *(shift + i);
        }
    }
    // otherwise shift to right -n times
    else
    {
        // convert n to positive
        n = n * -1;
        int shift_len = strlen(shift);
        for (int i = 0; i < shift_len + 1; i++)
        {
            *(shift + shift_len + n - i) = *(shift + shift_len - i);
        }
    }
    // if n is 0 we dont need to do any shifting.

    // after the shifting is done, we copy replacement into its place.
    for (int i = 0; i < strlen(replacement); i++)
    {
        *(found + i) = *(replacement + i);
    }

    // return 1 since the replacement is succesful.
    return 1;
}

void replace_all(char *main, const char *target, const char *replacement)
{
    int a = 1;

    while (a != 0)
    {
        a = replace(main, target, replacement);
    }
}

void print_spaces(int count)
{
    for (int i = 0; i < count; i++)
    {
        printf(" ");
    }
}
