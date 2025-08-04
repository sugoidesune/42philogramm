#ifndef PHILOGRAMM_H
#define PHILOGRAMM_H


#define MAX_PHILOS 300
#define MAX_ACTIONS 1000
#define MAX_LINE 128

extern int RESOLUTION;
extern int IGNORE_SHORT_ACTIONS;

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
} Philosopher;

void print_help_message(void);
const char* action_color(ActionType type);
#endif
