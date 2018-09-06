#ifndef UBW_VGA_H_
#define UBW_VGA_H_

void SetVideoMemAddr(void *addr);
void *GetVideoMem();
void SetVGAStatus(int rotated, int enabled);

#endif // UBW_VGA_H_
