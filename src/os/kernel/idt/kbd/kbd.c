#include"kbd.h"
#include"../../serial/serial.h"
bool shift = false;
bool caps_lock = false;

char *place = (char*)0x7E21;
uint8_t *placeidx = (uint8_t*)0x7E20;

void init_kbd(){
    outb(0x21, 0xFD); // 11111101 → enable IRQ1 (keyboard)
    outb(0xA1, 0xFF); // disable all slave IRQs
    placeidx[0] = 0;
    place[placeidx[0]] = '0';
}

void kbd_handler_C(uint8_t scancode){ //! this sends EOI in the asm
    char scancode_map[128] = {
        0, 27, '1','2','3','4','5','6','7','8','9','0','-','=','\b', // 0x00 - 0x0E
        '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,  // 0x0F - 0x1D
        'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\','z',   // 0x1E - 0x2C
        'x','c','v','b','n','m',',','.','/',0,'*',0,' ',0             // 0x2D - 0x39
    };
    bool released = scancode & 0x80;
    uint8_t code = scancode & 0x7F;

    // SHIFT keys
    if(code == 0x2A || code == 0x36){ // left or right shift
        shift = !released;
        return;
    }

    // CAPS LOCK key
    if(code == 0x3A && !released){
        caps_lock = !caps_lock;
        return;
    }

    // ignore releases for normal keys
    if(released) return;

    // map key
    char c = scancode_map[code];
    if(!c) return;

    // handle shift + caps
    bool upper = false;
    if(c >= 'a' && c <= 'z'){
        upper = (caps_lock ^ shift);
        if(upper) c -= 32; // a→A
    } else if(shift){ 
        // handle shifted symbols
        switch(c){
            case '1': c='!'; break;
            case '2': c='@'; break;
            case '3': c='#'; break;
            case '4': c='$'; break;
            case '5': c='%'; break;
            case '6': c='^'; break;
            case '7': c='&'; break;
            case '8': c='*'; break;
            case '9': c='('; break;
            case '0': c=')'; break;
            case '-': c='_'; break;
            case '=': c='+'; break;
            case '[': c='{'; break;
            case ']': c='}'; break;
            case '\\': c='|'; break;
            case ';': c=':'; break;
            case '\'': c='"'; break;
            case ',': c='<'; break;
            case '.': c='>'; break;
            case '/': c='?'; break;
        }
    }
    place[placeidx[0]] = c;
    placeidx[0]++;
    place[placeidx[0]] = '\0';
}
