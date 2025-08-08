// Print fork log row for a philosopher
#include "philogramm.h"

void print_fork_log(Philosopher *p) {
    // Print fork pickup arrows (↑) aligned to the correct time, on a new row below the chart
    // Print chart prefix (same as print_chart)
    int id = p->id;
    char prefix[4] = "";
    if (id < 10)
        strcpy(prefix, "   ");
    else if (id < 100)
        strcpy(prefix, "  ");
    printf("%s", prefix);
    extern int RESOLUTION;
    extern bool SHOW_EATCOUNT;

    // For each column, print the correct arrow (with color) if a fork was picked up at that time
    int col = 0;
    int fork_row_width = 0;
    while (col < MAX_BAR_CHARS) {
        int found = 0;
        for (int f = 0; f < p->fork_count; f++) {
            int arrow_pos = p->fork_times[f] / RESOLUTION;
            if (arrow_pos == col) {
                // Print colored arrow based on fork side
                if (p->fork_sides[f] == FORK_LEFT) {
                    printf(FORK_LEFT_COLOR "↑" COLOR_RESET); // blue for left
                } else if (p->fork_sides[f] == FORK_RIGHT) {
                    printf(FORK_RIGHT_COLOR "↑" COLOR_RESET); // magenta for right
                } else {
                    printf(FORK_UNKNOWN_COLOR "↑" COLOR_RESET); // default color
                }
                fork_row_width++;
                found = 1;
            }
        }
        if (!found) {
            printf(" ");
            fork_row_width++;
        }
        col++;
    }
    // fork_row_width now contains the total number of characters written (arrows and spaces)
    // Print empty eatcount borders directly after the fork arrows, on the same line
    int extra_spaces_for_border = 0;
    if((MAX_BAR_CHARS + 3) > fork_row_width) {
        extra_spaces_for_border = MAX_BAR_CHARS + 3 - fork_row_width;
        for (int i = 0; i < extra_spaces_for_border; i++) putchar(' ');
    }
    if (SHOW_EATCOUNT) {
        printf(" │");
        for (int i = 0; i < EATCOUNT_BLOCK_WIDTH - 2; i++) putchar(' ');
        printf("│");
    }
    putchar('\n');
}
