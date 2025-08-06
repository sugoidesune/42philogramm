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

    int arrow_cols[MAX_ACTIONS] = {0};
    int arrow_count = 0;
    for (int f = 0; f < p->fork_count; f++) {
        int arrow_pos = p->fork_times[f] / RESOLUTION;
        if (arrow_pos < MAX_BAR_CHARS) {
            arrow_cols[arrow_count++] = arrow_pos;
        }
    }
    int col = 0;
    int fork_row_width = 0; // Count of characters written (arrows and spaces)
    while (col < MAX_BAR_CHARS) {
        int arrows_here = 0;
        for (int a = 0; a < arrow_count; a++) {
            if (arrow_cols[a] == col) arrows_here++;
        }
        for (int k = 0; k < arrows_here; k++) {
            printf("↑");
            fork_row_width++;
        }
        if (!arrows_here) {
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
