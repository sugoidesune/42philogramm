// Print fork log row for a philosopher
#include "philogramm.h"

void print_fork_log(Philosopher *p) {
    printf("     "); // align with chart
    //printf("🍴 ");
    for (int i = 0; i < p->fork_count; i++) {
        char buf[16];
        //int n = snprintf(buf, sizeof(buf), "%dms", p->fork_times[i]);
        // /int pad = DEATH_MSG_WIDTH - n;
        // Use a fixed color block, similar to death log
        printf("%s  %s  %s", "\033[48;5;244;30m", buf, COLOR_RESET); // grey bg, black text
        //for (int j = 0; j < pad; j++) putchar(' ');
        // Add uncolored space between blocks
        // if (i < p->fork_count - 1) putchar(' ');
    }
    printf("\n");
}
