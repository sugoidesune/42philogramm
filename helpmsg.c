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
    printf("%s     120ms     %s", action_color(THINK), COLOR_RESET);
    printf("%s    80ms    %s", action_color(EAT), COLOR_RESET);
    printf("%s   60ms   %s", action_color(SLEEP), COLOR_RESET);
    printf("%s       %s", action_color(THINK), COLOR_RESET); // unfinished action, no duration
    printf("\n");
    printf("    \033[1m Philosopher's last (unfinished) action. 🠅 \033[0m\n");
    printf("\n\n");

    printf("     ┌───────────────────────── Fork Chart ──────────────────────────┐\n");
    printf("     │  Forks are shown as arrows between each philosopher           │\n");
    printf("     │  pointing to the philosopher who picked them up.              │\n");
    printf("     │  ⚠️  Important: Needs 'left' or 'right' in fork print message  │\n");
    printf("     └───────────────────────────────────────────────────────────────┘\n");
    printf("\n");

    // Upward arrow pointing to last action, with text on the right
    // There is no single Unicode character for a "right and up" arrow (↱ is the closest: "NORTH EAST ARROW").
    // You can use ↱ (U+2191 U+2197) or ↗ (U+2197) for a north-east arrow.                                   ");


    // Example bar for philosopher 3 (forks demo)
    printf("         "FORK_LEFT_COLOR" 🠯 "COLOR_RESET"◄—  Philosopher 3 picked up left fork (the right fork of Philosopher 2)\n");

    printf("  \033[1m3\033[0m: ");
    printf("%s       90ms       %s", action_color(THINK), COLOR_RESET);
    printf("%s    70ms    %s", action_color(EAT), COLOR_RESET);
    printf("%s    60ms    %s", action_color(SLEEP), COLOR_RESET);
    printf("\n");
    printf("    "FORK_RIGHT_COLOR" 🠭 "COLOR_RESET"◄—  Philosopher 3 picked up right fork (the left fork of Philosopher 4)\n");
    printf("  \033[1m4\033[0m: ");
    printf("%s       90ms       %s", action_color(EAT), COLOR_RESET);
    printf("%s    60ms    %s", action_color(SLEEP), COLOR_RESET);
    printf("%s    60ms    %s", action_color(THINK), COLOR_RESET);
    printf("\n\n");
    printf("  " FORK_LEFT_COLOR "🠭" COLOR_RESET ": Left Fork\n");
    printf("  " FORK_RIGHT_COLOR "🠭" COLOR_RESET ": Right Fork\n");
    printf("  " FORK_UNKNOWN_COLOR "🠭" COLOR_RESET ": A Fork\n\n");

    // Options box (same style as legend)
    printf("     \033[1mOptions\033[0m                                 \n");
    printf("     ┌───────────────┬────────────────────────────────────────────────┐\n");
    printf("     │  <resolution> │ Scale the size of chart. Possible values:      │\n");
    printf("     │               │ 5, 10, 20, 40 (default: 10)                    │\n");
    printf("     │               │                                                │\n");
    printf("     │  -i           │ Ignore actions of 3ms or less in chart output  │\n");
    printf("     │  -f           │ Turn on the fork chart                         │\n");
    printf("     │  -c           │ Turn off the eat count                         │\n");
    printf("     │  -s           │ Turn off the scale                             │\n");
    printf("     └───────────────┴────────────────────────────────────────────────┘\n");
    printf("\n");

    // Example usage segment
    printf("Example usage:\n");
    printf("  ./philo 6 400 200 200 5 | ./philogramm\n");
    printf("  ./philo 6 400 200 200 | head -n 50 | ./philogramm\n");
    printf("  cat philo_output | ./philogramm\n");
    printf("  cat philo_output | ./philogramm 20 -i\n");
    printf("  ./philo 6 400 200 200 | ./philogramm 20 -i -s -c -f\n");
    printf("\n");
}


