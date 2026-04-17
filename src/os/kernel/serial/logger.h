#pragma once

#define LOG_GREEN "\033[38;5;46m"
#define LOG_RED "\033[38;5;9m"
#define LOG_TERMINATE "\e[0m"

void LOG_info(const char *s);
void LOG_error(const char *s);
