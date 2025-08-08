// ...existing code...
// ...existing code...


#include "philogramm.h"
int RESOLUTION = 10;
bool IGNORE_SHORT_ACTIONS = false;
bool LOG_FORKS = false;
bool SHOW_SCALE = true; 
bool SHOW_EATCOUNT = true;
int MAX_BAR_CHARS = 100;

int calc_eatcount_spacing(Philosopher *p, int max_bar_chars, int resolution) {
    int first_action_start = (p->action_count > 0) ? p->actions[0].start : 0;
    int row_chars = 0;
    row_chars += first_action_start / resolution;
    int last_action_idx = p->action_count - 1;
    int is_dead = 0;
    if (last_action_idx >= 0 && p->actions[last_action_idx].type == DEAD) {
        is_dead = 1;
    }
    for (int j = 0; j < p->action_count; j++) {
        int duration = (p->actions[j].end > p->actions[j].start) ? (p->actions[j].end - p->actions[j].start) : 0;
        int rounded_duration = ((duration + resolution / 2) / resolution) * resolution;
        int len = 0;
        char duration_str[16];
        snprintf(duration_str, sizeof(duration_str), "%dms", duration);
        len = strlen(duration_str);
        int short_bar_limit = resolution * len;
        if (p->actions[j].type == DEAD) {
            // Account for death message width instead of bar
            // Format: "%4dms 🕱  " (4 digits, ms, emoji, 2 spaces)
            // Example: " 1234ms 🕱  " = 10 chars (excluding color codes)
            row_chars += 8;
            continue;
        }
        if (duration <= short_bar_limit) {
            int max_chars = (rounded_duration / resolution);
            if (max_chars < 1) max_chars = 1;
            if (max_chars > len) max_chars = len;
            row_chars += max_chars;
        } else {
            int bar_width = rounded_duration / resolution;
            if (bar_width < len) bar_width = len;
            row_chars += bar_width;
        }
    }
    // The eatcount block should start at the same column for all rows
    int spacing = max_bar_chars - row_chars;
    if (spacing < 0) spacing = 0;
    return spacing;
}




Philosopher* find_or_create_philo(Philosopher philos[], int *count, int id) {
    for (int i = 0; i < *count; i++) {
        if (philos[i].id == id) return &philos[i];
    }
    Philosopher *p = &philos[*count];
    p->id = id;
    p->action_count = 0;
    p->last_time = -1;
    p->last_type = NONE;
    p->died_time = -1;
    p->last_eat_time = -1;
    p->fork_count = 0;
    memset(p->fork_times, 0, sizeof(p->fork_times));
    p->eat_count = 0;
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
    // Track last eat time for log window
    if (type == EAT) {
        p->last_eat_time = time;
        p->eat_count++;
    }
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

void print_chart(Philosopher *p, int max_bar_chars) {
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
        // ...existing code...
        if (IGNORE_SHORT_ACTIONS && duration > 0 && duration <= 3) {
            continue;
        }
        if (a->type == DEAD) {
            int died_time = a->start;
            printf("%s%4dms 🕱  %s", action_color(DEAD), died_time, COLOR_RESET);
            continue;
        }
        if (duration == 0) {
            printf("%s   %s", action_color(a->type), COLOR_RESET);
            continue;
        }
        char duration_str[16];
        snprintf(duration_str, sizeof(duration_str), "%dms", duration);
        int len = strlen(duration_str);
        int short_bar_limit = RESOLUTION * strlen(duration_str);
        int rounded_duration = ((duration + RESOLUTION / 2) / RESOLUTION) * RESOLUTION;
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
    // Calculate spacing for eat count block
    if (SHOW_EATCOUNT) {
        int spacing = calc_eatcount_spacing(p, max_bar_chars, RESOLUTION);
        for (int s = 0; s < spacing; s++) printf("\033[38;5;240m┈\033[0m");
        printf("┤ ate %d times %s│\n", p->eat_count, p->eat_count > 9 ? "" : " ");
    } else {
        putchar('\n');
    }
}

void print_death_msg(int ms) {
    char buf[32];
    int n = snprintf(buf, sizeof(buf), "🕱 %dms", ms);
    int pad = DEATH_MSG_WIDTH - n;
    printf("%s  %s  %s", action_color(DEAD), buf, COLOR_RESET);
    for (int i = 0; i < pad; i++) putchar(' ');
}

void print_meal_msg(int ms) {
    char buf[32];
    int n = snprintf(buf, sizeof(buf), "%dms", ms);
    int pad = MEAL_MSG_WIDTH - n;
    printf("%s  %s  %s", action_color(EAT), buf, COLOR_RESET);
    for (int i = 0; i < pad; i++) putchar(' ');
}

void process_line(char *line, Philosopher philos[], int *philo_count) {
    int time, id;
    char action_str[MAX_LINE];
    if (sscanf(line, "%d %d %[^\n]", &time, &id, action_str) == 3) {
        ActionType type = parse_action(action_str);
        Philosopher *p = find_or_create_philo(philos, philo_count, id);
        add_action(p, time, type);
            // Track death time for log window
            if (type == DEAD) {
                p->died_time = time;
            }
            // Track fork pickup times if LOG_FORKS is enabled and action contains "fork"
            if (LOG_FORKS && strstr(action_str, "fork")) {
                if (p->fork_count < MAX_ACTIONS) {
                    p->fork_times[p->fork_count++] = time;
                }
            }
    }
}



void parse_options(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            print_help_message();
            exit(0);
        } else if (strcmp(argv[i], "-i") == 0) {
            IGNORE_SHORT_ACTIONS = true;
        } else if (strcmp(argv[i], "-f") == 0) {
            LOG_FORKS = true;
        } else if (strcmp(argv[i], "-s") == 0) {
            SHOW_SCALE = false;
        } else if (strcmp(argv[i], "-c") == 0) {
            SHOW_EATCOUNT = false;
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


int calc_max_bar_chars(Philosopher philos[], int philo_count, int resolution) {
    int max_bar_chars = 0;
    for (int i = 0; i < philo_count; i++) {
        Philosopher *p = &philos[i];
        int row_chars = 0;
        int first_action_start = (p->action_count > 0) ? p->actions[0].start : 0;
        row_chars += first_action_start / resolution;
        for (int j = 0; j < p->action_count; j++) {
            int duration = (p->actions[j].end > p->actions[j].start) ? (p->actions[j].end - p->actions[j].start) : 0;
            int rounded_duration = ((duration + resolution / 2) / resolution) * resolution;
            int len = 0;
            char duration_str[16];
            snprintf(duration_str, sizeof(duration_str), "%dms", duration);
            len = strlen(duration_str);
            int short_bar_limit = resolution * len;
            if (duration <= short_bar_limit) {
                int max_chars = (rounded_duration / resolution);
                if (max_chars < 1) max_chars = 1;
                if (max_chars > len) max_chars = len;
                row_chars += max_chars;
            } else {
                int bar_width = rounded_duration / resolution;
                if (bar_width < len) bar_width = len;
                row_chars += bar_width;
            }
        }
        if (row_chars > max_bar_chars) max_bar_chars = row_chars;
    }
    max_bar_chars += 10; // Account for scale labels
    return max_bar_chars;
}

void print_scale(int max_bar_chars, int resolution) {
    printf("     "); // align with chart prefix
    for (int i = 0; i < max_bar_chars; i++) {
        int ms = i * resolution;
        if (i == 0 || i == max_bar_chars - 1 || i % 10 == 0) {
            printf("%-4d", ms);
            i += 3; // skip next 3 chars for label width
        } else {
            printf(" ");
        }
    }
    printf("\n    ▕");
    // Decrement max bar chars to remove the last "⠂" after the last number.
    max_bar_chars--;
    for (int i = 0; i < max_bar_chars; i++) {
        if (i % 10 == 0)
            printf("▏");
        else if (i % 10 == 9)
            printf("▕");
        else
            printf("⠄");
    }
    if(SHOW_EATCOUNT)
        printf("   ╭──────────────╮\n");
    else
        printf("\n");
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
    MAX_BAR_CHARS = calc_max_bar_chars(philos, philo_count, RESOLUTION);
        if (SHOW_SCALE) {
            print_scale(MAX_BAR_CHARS, RESOLUTION);
        } else if (SHOW_EATCOUNT) {
            // Print top border for 'ate X times' messages if scale is off
            printf("    "); // align with chart prefix
            for (int i = 0; i < MAX_BAR_CHARS - 1; i++) putchar(' ');
            printf("    ╭──────────────╮\n");
        }
    // Print charts in sorted order
    for (int i = 0; i < philo_count; i++) {
        print_chart(&philos[i], MAX_BAR_CHARS);
        if (LOG_FORKS) {
            print_fork_log(&philos[i]);
        }
    }
        // Print bottom border for 'ate X times' messages if enabled
        if (SHOW_EATCOUNT) {
            printf("    "); // align with chart prefix
            for (int i = 0; i < MAX_BAR_CHARS - 1; i++) putchar(' ');
            printf("    ╰──────────────╯\n");
        }
    // Print death log if any philosopher died
    bool any_died = false;
    for (int i = 0; i < philo_count; i++) {
        Philosopher *p = &philos[i];
        if (p->last_type == DEAD && p->died_time != -1) {
            any_died = true;
            break;
        }
    }
    if (any_died) {
        printf("\n  ┌───────────────────────── Death Log ─────────────────────────┐\n");
        for (int i = 0; i < philo_count; i++) {
            Philosopher *p = &philos[i];
            if (p->last_type == DEAD && p->died_time != -1) {
                printf("  │ %s%2d%s  Died at  ", "\033[1m", p->id, "\033[0m");
                print_death_msg(p->died_time);
                printf("    │   Last ate at  ");
                if (p->last_eat_time != -1) {
                    print_meal_msg(p->last_eat_time);
                } else {
                    print_meal_msg(0);
                }
                printf(" │\n");
            }
        }
        printf("  └─────────────────────────────────────────────────────────────┘\n");
    }
    return 0;
}
