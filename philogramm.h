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

extern int RESOLUTION;
extern bool IGNORE_SHORT_ACTIONS;
extern bool LOG_FORKS;

// ANSI color codes
#define COLOR_EAT     "\033[1;31m"
#define COLOR_SLEEP   "\033[1;34m"
#define COLOR_THINK   "\033[1;32m"
#define COLOR_RESET   "\033[0m"

// Action types
typedef enum { EAT, SLEEP, THINK, DEAD, NONE } ActionType;

// Action struct
typedef struct {
    int start;
    int end;
    ActionType type;
} Action;

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
} Philosopher;

void print_help_message(void);
const char* action_color(ActionType type);
#endif
