#include "include/vga.h"
#include "include/soc.h"

void SetVideoMemAddr(size_t addr) {
    VGA_AR = addr & 0xfff00000;
}

void SetVGAStatus(int rotated, int enabled) {
    uint32_t status = rotated ? VGA_CR_DPS : 0;
    status |= enabled ? VGA_CR_TDE : 0;
    VGA_CR = status;
}
