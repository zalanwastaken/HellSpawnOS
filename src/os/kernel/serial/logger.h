#pragma once

#include<stdint.h>

#define LOG_GREEN "\033[38;5;46m"
#define LOG_RED "\033[38;5;9m"
#define LOG_YELLOW "\033[38;5;11m"
#define LOG_TERMINATE "\e[0m"

void LOG_init();
int LOG_format(char *buf, int max, const char *fmt, ...);
void LOGLN(const char *clr, const char *suffix, const char *s);
char* LOG_get_logged();
void LOG_HexLN(const char *clr, const char *suffix, uint32_t s);
void LOG_infoLN(const char *s);
void LOG_errorLN(const char *s);
//void LOG_info_hexLN(uint32_t s);
void LOG_warnLN(const char *s);
