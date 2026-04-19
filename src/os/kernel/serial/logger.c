#include"logger.h"
#include"serial.h"
#include"../mem_manager/manager.h"

#include<stdarg.h>

//* ---UTILS---

// append char with bounds check
void append_char(char *buf, int *pos, int max, char c) {
    if (*pos < max - 1) {
        buf[(*pos)++] = c;
    }
}

// append string
void append_str(char *buf, int *pos, int max, const char *s) {
    while (*s) {
        append_char(buf, pos, max, *s++);
    }
}

// append int
void append_int(char *buf, int *pos, int max, int num) {
    char tmp[12];
    int i = 0;

    if (num == 0) {
        append_char(buf, pos, max, '0');
        return;
    }

    if (num < 0) {
        append_char(buf, pos, max, '-');
        num = -num;
    }

    while (num > 0) {
        tmp[i++] = (num % 10) + '0';
        num /= 10;
    }

    while (i--) {
        append_char(buf, pos, max, tmp[i]);
    }
}

//* ---FUNCS---

size_t logged_size = 256; //? we allocate 256 but indexable are 255(starting from 0)
uint32_t loggedIDX = 0;
char *logged = (char*)-1;
void LOG_init(){
    logged = kalloc(sizeof(char)*256);
}

// main formatter
int LOG_format(char *buf, int max, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    int pos = 0;

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;

            if (*fmt == 's') {
                char *s = va_arg(args, char *);
                append_str(buf, &pos, max, s);
            } 
            else if (*fmt == 'd') {
                int d = va_arg(args, int);
                append_int(buf, &pos, max, d);
            } 
            else if (*fmt == 'c') {
                char c = (char)va_arg(args, int);
                append_char(buf, &pos, max, c);
            } 
            else {
                append_char(buf, &pos, max, '%');
                append_char(buf, &pos, max, *fmt);
            }
        } else {
            append_char(buf, &pos, max, *fmt);
        }

        fmt++;
    }

    buf[pos] = '\0';
    va_end(args);

    return pos;
}

void LOGLN(const char *clr, const char *suffix, const char *s){
    serial_print(clr);
    serial_print(suffix);
    serial_print(s);
    serial_printLN(LOG_TERMINATE);

    if(logged == (char*)-1){
        serial_printLN("Waiting for log init not adding last message");
    }else{
        for(uint32_t i = 0; i<logged_size; i++){
            if(loggedIDX >= logged_size){
                char *newlogged = alloc(getHeapData(), logged_size+256);
                logged_size += 256;
                for(uint32_t f = 0; f<loggedIDX; f++){
                    newlogged[f] = logged[f];
                }
                kfree(logged);
                logged = newlogged;
                serial_print("New memory allocated: ");
                serial_print_hexLN(logged_size);
            }
            logged[loggedIDX] = s[i];
            loggedIDX++;
            if(s[i] == '\0'){
                break;
            }
        }
    }
}

char* LOG_get_logged(){
    return logged;
}

void LOG_HexLN(const char *clr, const char *suffix, uint32_t s){
    serial_print(clr);
    serial_print(suffix);
    serial_print_hex(s);
    serial_printLN(LOG_TERMINATE);
}

void LOG_infoLN(const char *s){
    LOGLN(LOG_GREEN, "[INFO] ", s);
}

void LOG_errorLN(const char *s){
    LOGLN(LOG_RED, "[ERROR] ", s);
}

/*
void LOG_info_hexLN(uint32_t s){
    LOG_HexLN(LOG_GREEN, "[INFO] ", s);
}
*/

void LOG_warnLN(const char *s){
    LOGLN(LOG_YELLOW, "[WARN] ", s);
}
