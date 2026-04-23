#pragma once

#include<stdint.h>
#include<stddef.h>

#define SCHEDULER_STATE_RUNNING 0x01
#define SCHEDULER_STATE_STOPPED 0x02

typedef struct {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
} regs;

typedef struct task_t{
    uint32_t *strack;
    regs *registers;
    uint16_t id;
    uint8_t state;
};

void scheduler_pit_catch(regs *r);
