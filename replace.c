#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

void append(char **buffer, size_t *buf_size, size_t *buf_len, const char *str, size_t len) {
    if (*buf_len + len >= *buf_size) {
        size_t new_size = (*buf_size * 2) + len + 1;
        char *new_buf = realloc(*buffer, new_size);
        if (!new_buf) {
            perror("Failed to reallocate memory");
            exit(1);
        }
        *buffer = new_buf;
        *buf_size = new_size;
    }
    
    if (len > 0) {
        strncpy(*buffer + *buf_len, str, len);
        *buf_len += len;
    }
    (*buffer)[*buf_len] = '\0';
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <regex> <text> <replacement>\n", argv[0]);
        return 1;
    }

    const char *pattern = argv[1];
    const char *text = argv[2];
    const char *replacement = argv[3];

    regex_t regex;
    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        fprintf(stderr, "Could not compile regex\n");
        return 1;
    }

    size_t buf_size = 100;
    size_t buf_len = 0;
    char *buffer = malloc(buf_size);
    if (!buffer) {
        perror("Failed to allocate memory");
        regfree(&regex);
        return 1;
    }
    buffer[0] = '\0';

    size_t text_len = strlen(text);
    size_t current_pos = 0;
    regmatch_t match;

    while (current_pos <= text_len) {
        int reti = regexec(&regex, text + current_pos, 1, &match, 0);

        if (reti == REG_NOMATCH) {
            append(&buffer, &buf_size, &buf_len, text + current_pos, text_len - current_pos);
            break;
        }

        append(&buffer, &buf_size, &buf_len, text + current_pos, match.rm_so);

        append(&buffer, &buf_size, &buf_len, replacement, strlen(replacement));

        size_t match_len = match.rm_eo - match.rm_so;
        current_pos += match.rm_eo;

        if (match_len == 0) {
            if (current_pos < text_len) {
                append(&buffer, &buf_size, &buf_len, text + current_pos, 1);
                current_pos++;
            } else {
                break;
            }
        }
    }

    printf("%s\n", buffer);

    free(buffer);
    regfree(&regex);

    return 0;
}
