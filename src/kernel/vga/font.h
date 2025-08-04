#pragma once
#include<stdint.h>

void draw_char(char c, int x, int y, uint32_t color);
void draw_string(const char *s, int x, int y, uint32_t color);
void draw_string_scaled(const char *s, int x, int y, uint32_t color, int scale);
