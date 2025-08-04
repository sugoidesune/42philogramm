// Helper for philosopher label background



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// include for isatty
#include <unistd.h>
#include "philogramm.h"

int RESOLUTION = 10;
bool IGNORE_SHORT_ACTIONS = false;
bool VISUALIZE_FORKS = false;

Philosopher* find_or_create_philo(Philosopher philos[], int *count, int id) {
    for (int i = 0; i < *count; i++) {
        if (philos[i].id == id) return &philos[i];
    }
    Philosopher *p = &philos[*count];
    p->id = id;
    p->action_count = 0;
    p->last_time = -1;
    p->last_type = NONE;
    (*count)++;
    return p;
}

ActionType parse_action(const char *str) {
    if (strstr(str, "die")) return DEAD;
    if (strstr(str, "eat")) return EAT;
    if (strstr(str, "sleep")) return SLEEP;
    if (strstr(str, "think")) return THINK;
    return NONE;
}

const char* action_color(ActionType type) {
    // 255 color support: EAT=red(215), SLEEP=blue(38), THINK=green(35), DEAD=black(232)
    if (type == EAT) return "\033[48;5;209;30m"; // orange-red bg, black text
    if (type == SLEEP) return "\033[48;5;110;30m"; // blue bg, black text 44
    if (type == THINK) return "\033[48;5;149;30m"; // green bg, black text
    if (type == DEAD) return "\033[48;5;232;37m"; // true black bg, light grey text
    return COLOR_RESET;
}

const char* action_color2(ActionType type) {
    if (type == EAT) return "\033[101;30m"; // light red bg, black text
    if (type == SLEEP) return "\033[104;30m"; // light blue bg, black text
    if (type == THINK) return "\033[102;30m"; // light green bg, black text
    if (type == DEAD) return "\033[48;5;232;37m"; // true black bg, light grey text
    return COLOR_RESET;
}

void add_action(Philosopher *p, int time, ActionType type) {
    if (type == NONE) return;
    if (p->last_type != NONE && p->last_type != type) {
        // End previous action
        if (p->action_count < MAX_ACTIONS) {
            p->actions[p->action_count - 1].end = time;
        }
    }
    if (p->last_type != type) {
        // Start new action
        if (p->action_count < MAX_ACTIONS) {
            p->actions[p->action_count].start = time;
            p->actions[p->action_count].type = type;
            p->actions[p->action_count].end = -1;
            p->action_count++;
        }
    }
    p->last_time = time;
    p->last_type = type;
}

void finalize_actions(Philosopher *p) {
    if (p->action_count > 0 && p->actions[p->action_count - 1].end == -1) {
        p->actions[p->action_count - 1].end = p->last_time;
    }
}

void print_chart(Philosopher *p) {
    int id = p->id;
    char prefix[4] = "";
    if (id < 10)
        strcpy(prefix, "  ");
    else if (id < 100)
        strcpy(prefix, " ");
    // Print prefix, number, colon, and space all on dark grey background, then reset
    printf("%s\033[1m%d: \033[0m", prefix, id);
    int first_action_start = (p->action_count > 0) ? p->actions[0].start : 0;
    if (first_action_start > 0) {
        int dots = first_action_start / RESOLUTION;
        for (int d = 0; d < dots; d++) putchar('.');
    }

    // Helper: detect if last action is zero-duration and not DEAD
    int last_action_idx = p->action_count - 1;
    int last_is_zero = 0;
    ActionType last_type = NONE;
    if (last_action_idx >= 0) {
        Action *last = &p->actions[last_action_idx];
        if (last->type != DEAD && (last->end <= last->start)) {
            last_is_zero = 1;
            last_type = last->type;
        }
    }


    for (int i = 0; i < p->action_count; i++) {
        Action *a = &p->actions[i];
        int duration = (a->end > a->start) ? (a->end - a->start) : 0;
        // Use rounding for bar width calculation
        int rounded_duration = ((duration + RESOLUTION / 2) / RESOLUTION) * RESOLUTION;
        if (IGNORE_SHORT_ACTIONS && duration > 0 && duration <= 3) {
            continue;
        }
        if (a->type == DEAD) {
            // Print timestamp before skull emoji
            int died_time = a->start;
            printf("%s%4dms 🕱  %s", action_color(DEAD), died_time, COLOR_RESET);
            continue;
        }
        if (duration == 0) {
            // Only print X if this is the last action and it is not DEAD
            if (i == last_action_idx && last_is_zero) {
                printf("%s   %s", action_color(last_type), COLOR_RESET);
            }
            continue;
        }
        char duration_str[16];
        snprintf(duration_str, sizeof(duration_str), "%dms", duration);
        int len = strlen(duration_str);
        // Special handling for short durations
        int short_bar_limit = RESOLUTION * strlen(duration_str); // max chars based on ms per hash
        if (duration <= short_bar_limit) {
            int max_chars = (rounded_duration / RESOLUTION);
            if (max_chars < 1) max_chars = 1;
            if (max_chars > len) max_chars = len;
            const char *bg_color = action_color(a->type);
            printf("%s", bg_color);
            for (int c = 0; c < max_chars; c++) putchar(duration_str[c]);
            printf("%s", COLOR_RESET);
            continue;
        }
        // Normal bar for longer durations
        int bar_width = rounded_duration / RESOLUTION;
        if (bar_width < len) bar_width = len;
        char bar[bar_width + 1];
        memset(bar, ' ', bar_width);
        bar[bar_width] = '\0';
        int start = (bar_width - len) / 2;
        if (start < 0) start = 0;
        int copy_len = len;
        if (start + copy_len > bar_width) copy_len = bar_width - start;
        memcpy(bar + start, duration_str, copy_len);
        const char *bg_color = action_color(a->type);
        printf("%s%s%s", bg_color, bar, COLOR_RESET);
    }
    printf("\n");
}

void process_line(char *line, Philosopher philos[], int *philo_count) {
    int time, id;
    char action_str[MAX_LINE];
    if (sscanf(line, "%d %d %[^\n]", &time, &id, action_str) == 3) {
        ActionType type = parse_action(action_str);
        Philosopher *p = find_or_create_philo(philos, philo_count, id);
        add_action(p, time, type);
    }
}



void parse_options(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            print_help_message();
            exit(0);
        } else if (strcmp(argv[i], "-i") == 0) {
            IGNORE_SHORT_ACTIONS = true;
        } else {
            // Check if argument is a positive integer (resolution)
            char *endptr;
            long val = strtol(argv[i], &endptr, 10);
            if (*endptr == '\0' && val > 0) {
                RESOLUTION = (int)val;
            }
        }
    }
}

int main(int argc, char **argv) {
    Philosopher philos[MAX_PHILOS];
    int philo_count = 0;
    char line[MAX_LINE];
    parse_options(argc, argv);
    // Exit if no input is being piped
    if (isatty(fileno(stdin))) {
        fprintf(stderr, "No input detected. Please pipe philosopher logs to visualize.\n");
        return 1;
    }
    while (fgets(line, sizeof(line), stdin)) {
        process_line(line, philos, &philo_count);
    }
    // Finalize actions
    for (int i = 0; i < philo_count; i++) {
        finalize_actions(&philos[i]);
    }
    // Sort philosophers by id ascending
    for (int i = 0; i < philo_count - 1; i++) {
        for (int j = i + 1; j < philo_count; j++) {
            if (philos[i].id > philos[j].id) {
                Philosopher tmp = philos[i];
                philos[i] = philos[j];
                philos[j] = tmp;
            }
        }
    }
    // Print charts in sorted order
    for (int i = 0; i < philo_count; i++) {
        print_chart(&philos[i]);
    }
    return 0;
}
