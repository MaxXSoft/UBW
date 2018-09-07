#include "../include/vga.h"
#include "../include/type.h"
#include "../include/soc.h"

static void *video_mem = (void *)0x85000000;

void SetVideoMemAddr(void *addr) {
    video_mem = (void *)((size_t)addr & 0x3ff00000);
    VGA_AR = (size_t)video_mem;
    // make sure the memory address is in kseg0
    video_mem = (void *)((size_t)video_mem | 0x80000000);
}

void *GetVideoMem() {
    return video_mem;
}

void SetVGAStatus(int rotated, int enabled) {
    uint32_t status = rotated ? VGA_CR_DPS : 0;
    status |= enabled ? VGA_CR_TDE : 0;
    VGA_CR = status;
}
