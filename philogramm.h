#ifndef PHILOGRAMM_H
#define PHILOGRAMM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// include for isatty
#include <unistd.h>


#define MAX_PHILOS 300
#define MAX_ACTIONS 1000
#define MAX_LINE 128

#define DEATH_MSG_WIDTH 12
#define MEAL_MSG_WIDTH 8
#define EATCOUNT_BLOCK_WIDTH 16

extern int RESOLUTION;
extern bool IGNORE_SHORT_ACTIONS;
extern bool LOG_FORKS;
extern bool SHOW_SCALE;
extern bool SHOW_EATCOUNT;
extern int MAX_BAR_CHARS;

// ANSI color codes
#define COLOR_EAT     "\033[1;31m"
#define COLOR_SLEEP   "\033[1;34m"
#define COLOR_THINK   "\033[1;32m"
#define COLOR_RESET   "\033[0m"

// Fork arrow colors
#define FORK_LEFT_COLOR   "\033[1;33m" // yellow (as orange substitute)
#define FORK_RIGHT_COLOR  "\033[38;5;49m"  // teal (xterm 49)
#define FORK_UNKNOWN_COLOR "\033[1m"   // bold (default)

// Action types
typedef enum { EAT, SLEEP, THINK, DEAD, NONE } ActionType;

// Action struct
typedef struct {
    int start;
    int end;
    ActionType type;
} Action;


typedef enum { FORK_NONE = 0, FORK_LEFT, FORK_RIGHT } ForkSide;

// Philosopher struct
typedef struct {
    int id;
    int action_count;
    Action actions[MAX_ACTIONS];
    int last_time;
    ActionType last_type;
    int died_time; // -1 if not dead
    int last_eat_time; // -1 if never ate
    int fork_count;
    int fork_times[MAX_ACTIONS]; // times when forks were picked up
    ForkSide fork_sides[MAX_ACTIONS]; // left/right/none for each fork pickup
    int eat_count; // number of times philosopher ate
} Philosopher;

void print_help_message(void);
const char* action_color(ActionType type);
void print_fork_row(Philosopher *philos, int philo_count, int fork_idx);
void print_scale(int max_bar_chars, int resolution);
int calc_max_bar_chars(Philosopher philos[], int philo_count, int resolution);
void print_chart(Philosopher *p, int max_bar_chars);
#endif
