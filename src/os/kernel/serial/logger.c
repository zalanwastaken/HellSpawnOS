#include"logger.h"
#include"serial.h"

void LOG_info(const char *s){
    serial_print(LOG_GREEN);
    serial_print("[INFO] ");
    serial_print(s);
    serial_printLN(LOG_TERMINATE);
}

void LOG_error(const char *s){
    serial_print(LOG_RED);
    serial_print("[ERROR] ");
    serial_print(s);
    serial_printLN(LOG_TERMINATE);
}
