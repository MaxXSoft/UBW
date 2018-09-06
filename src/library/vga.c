#include "../include/vga.h"
#include "../include/soc.h"

static void *video_mem = 0x05000000;

void SetVideoMemAddr(size_t addr) {
    video_mem = addr & 0xfff00000;
    VGA_AR = video_mem;
}

void *GetVideoMem() {
    return video_mem;
}

void SetVGAStatus(int rotated, int enabled) {
    uint32_t status = rotated ? VGA_CR_DPS : 0;
    status |= enabled ? VGA_CR_TDE : 0;
    VGA_CR = status;
}
