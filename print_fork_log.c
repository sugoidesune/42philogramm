// Print fork log row for a philosopher
#include "philogramm.h"

// Print fork log row for the fork between philosopher i (upper) and i+1 (lower)
// philos: array of philosophers, philo_count: number of philosophers, fork_idx: which fork (0 = between 0 and 1)
void print_fork_row(Philosopher *philos, int philo_count, int fork_idx) {
    // Print prefix for alignment
    printf("     ");
    extern int RESOLUTION;
    extern bool SHOW_EATCOUNT;
    // For each column, print ↑ if upper picked up, ↓ if lower picked up, space otherwise
    int col = 0;
    int fork_row_width = 0;
    Philosopher *upper = &philos[fork_idx];
    Philosopher *lower = &philos[(fork_idx + 1) % philo_count];
    while (col < MAX_BAR_CHARS) {
        int found = 0;
        // Check if upper picked up left fork at this time
        for (int f = 0; f < upper->fork_count; f++) {
            int arrow_pos = upper->fork_times[f] / RESOLUTION;
            if (arrow_pos == col && upper->fork_sides[f] == FORK_LEFT) {
                printf(FORK_LEFT_COLOR "↑" COLOR_RESET);
                found = 1;
                break;
            }
        }
        // Check if lower picked up right fork at this time
        if (!found) {
            for (int f = 0; f < lower->fork_count; f++) {
                int arrow_pos = lower->fork_times[f] / RESOLUTION;
                if (arrow_pos == col && lower->fork_sides[f] == FORK_RIGHT) {
                    printf(FORK_RIGHT_COLOR "↓" COLOR_RESET);
                    found = 1;
                    break;
                }
            }
        }
        if (!found) {
            printf(" ");
        }
        fork_row_width++;
        col++;
    }
    // Print empty eatcount borders directly after the fork arrows, on the same line
    int extra_spaces_for_border = 0;
    if((MAX_BAR_CHARS + 3) > fork_row_width) {
        extra_spaces_for_border = MAX_BAR_CHARS + 1 - fork_row_width;
        for (int i = 0; i < extra_spaces_for_border; i++) putchar(' ');
    }
    if (SHOW_EATCOUNT) {
        printf(" │");
        for (int i = 0; i < EATCOUNT_BLOCK_WIDTH - 2; i++) putchar(' ');
        printf("│");
    }
    putchar('\n');
}
