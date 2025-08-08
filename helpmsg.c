#include <stdio.h>
#include "philogramm.h"

void print_help_message(void) {
    // Legend with pointer (aligned)
    printf("     ┌──────────┬────────────┬────────────┬────────┐\n");
    printf("     │ %s Eating %s │ %s Sleeping %s │ %s Thinking %s │ %s Dead %s │\n",
        action_color(EAT), COLOR_RESET,
        action_color(SLEEP), COLOR_RESET,
        action_color(THINK), COLOR_RESET,
        action_color(DEAD), COLOR_RESET);
    printf("     └──────────┴────────────┴────────────┴────────┘\n");
    printf("         │            │             │         │\n");
    printf("         ▼            ▼             ▼         ▼\n");
    // Example bar with philosopher number
    printf("  \033[1m1\033[0m: ");
    printf("%s    200ms    %s", action_color(EAT), COLOR_RESET);
    printf("%s    150ms    %s", action_color(SLEEP), COLOR_RESET);
    printf("%s    100ms    %s", action_color(THINK), COLOR_RESET);
    printf("%s  🕱   %s\n", action_color(DEAD), COLOR_RESET);
    printf("\n");

    // Example bar for philosopher 2 (starts with sleeping, ends with unfinished action)
    printf("  \033[1m2\033[0m: ");
    printf("%s   120ms   %s", action_color(SLEEP), COLOR_RESET);
    printf("%s   80ms   %s", action_color(EAT), COLOR_RESET);
    printf("%s   60ms   %s", action_color(THINK), COLOR_RESET);
    printf("%s       %s", action_color(SLEEP), COLOR_RESET); // unfinished action, no duration
    printf("\n");
    // Upward arrow pointing to last action, with text on the right
    printf("%s\033[1m▲\033[0m\n", "                                       ");
    printf("           \033[1m Philosophers last action. ─╯\033[0m\n");
    printf("\n");

    // Example bar for philosopher 3 (forks demo)
    printf("  \033[1m3\033[0m: ");
    printf("%s       90ms       %s", action_color(THINK), COLOR_RESET);
    printf("%s    70ms    %s", action_color(EAT), COLOR_RESET);
    printf("%s    60ms    %s", action_color(SLEEP), COLOR_RESET);
    printf("\n");
    printf("        " FORK_LEFT_COLOR "↑" COLOR_RESET "             " FORK_RIGHT_COLOR "↑" COLOR_RESET "              " FORK_UNKNOWN_COLOR "↑" COLOR_RESET "\n");
    printf("        left fork     right fork     a fork\n");
    printf("\n");

    // Options box (same style as legend)
    printf("     \033[1mOptions\033[0m                                 \n");
    printf("     ┌───────────────┬────────────────────────────────────────────────┐\n");
    printf("     │  <resolution> │ Scale the size of chart. Possible values:      │\n");
    printf("     │               │ 5, 10, 20, 40 (default: 10)                    │\n");
    printf("     │               │                                                │\n");
    printf("     │  -i           │ Ignore actions of 3ms or less in chart output  │\n");
    printf("     │  -f           │ Turn on the fork chart                        │\n");
    printf("     │  -c           │ Turn off the eat count                        │\n");
    printf("     │  -s           │ Turn off the scale                            │\n");
    printf("     └───────────────┴────────────────────────────────────────────────┘\n");
    printf("\n");

    // Example usage segment
    printf("Example usage:\n");
    printf("  ./philo 6 400 200 200 5 | ./visualize\n");
    printf("  ./philo 6 400 200 200 | head -n 50 | ./visualize\n");
    printf("  cat philo_output | ./visualize\n");
    printf("  cat philo_output | ./visualize 20 -i\n");
    printf("\n");
}


