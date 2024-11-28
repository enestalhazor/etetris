#ifndef ENES_UTIL
#define ENES_UTIL

#define ESCAPE "\x1b"
#define RESET_TEXT ESCAPE "[0m"
#define ITALIC_TEXT ESCAPE "[3m"
#define BOLD_TEXT ESCAPE "[1m"
#define HIGHLIGHTED_TEXT ESCAPE "[7m"
#define BLACK_TEXT ESCAPE "[30m"
#define RED_TEXT ESCAPE "[31m"
#define GREEN_TEXT ESCAPE "[32m"
#define YELLOW_TEXT ESCAPE "[33m"
#define BLUE_TEXT ESCAPE "[34m"
#define MAGENTA_TEXT ESCAPE "[35m"
#define CYAN_TEXT ESCAPE "[36m"
#define WHITE_TEXT ESCAPE "[37m"
#define BLACK_BRIGHT_TEXT ESCAPE "[90m"
#define RED_BRIGHT_TEXT ESCAPE "[91m"
#define GREEN_BRIGHT_TEXT ESCAPE "[92m"
#define YELLOW_BRIGHT_TEXT ESCAPE "[93m"
#define BLUE_BRIGHT_TEXT ESCAPE "[94m"
#define MAGENTA_BRIGHT_TEXT ESCAPE "[95m"
#define CYAN_BRIGHT_TEXT ESCAPE "[96m"
#define WHITE_BRIGHT_TEXT ESCAPE "[97m"
#define BLACK_BG ESCAPE "[40m"
#define RED_BG ESCAPE "[41m"
#define GREEN_BG ESCAPE "[42m"
#define YELLOW_BG ESCAPE "[43m"
#define BLUE_BG ESCAPE "[44m"
#define MAGENTA_BG ESCAPE "[45m"
#define CYAN_BG ESCAPE "[46m"
#define WHITE_BG ESCAPE "[47m"
#define BLACK_BRIGHT_BG ESCAPE "[100m"
#define RED_BRIGHT_BG ESCAPE "[101m"
#define GREEN_BRIGHT_BG ESCAPE "[102m"
#define YELLOW_BRIGHT_BG ESCAPE "[103m"
#define BLUE_BRIGHT_BG ESCAPE "[104m"
#define MAGENTA_BRIGHT_BG ESCAPE "[105m"
#define CYAN_BRIGHT_BG ESCAPE "[106m"
#define WHITE_BRIGHT_BG ESCAPE "[107m"
#define ERASE_LINE ESCAPE "[2K"


int get_random_number(int min, int max);

int power(int x, int y);

char *string(const char *s1, const char *w);

void read_string(char *wsentence);

void replace_all(char *main, const char *target, const char *replacement);

int replace(char *main, const char *target, const char *replacement);

int perform_race(int racer_count);

void print_spaces(int count);


struct horse
{
    int distance;
};


#endif