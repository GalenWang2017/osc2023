#ifndef _CPIO_H
#define _CPIO_H

// #define _initramfs_addr_ 0x8000000 // in qemu
#define _initramfs_addr_ 0x20000000 // in rpi3

#define _trailer_ "TRAILER!!!\0" 
struct cpio_newc_header;
void ls(char *addr);
void cat(char *addr, char * filename);
int memcmp(void *s1, void *s2, int n);
void memcpy(void *dst, void *src, int n);
char* ramfs_init(void);

#endif