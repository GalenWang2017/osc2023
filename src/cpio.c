#include "cpio.h"
#include "mini_uart.h"
#include "string.h"

#define ALIGN(num, base) ((num + base - 1) & ~(base - 1))

struct cpio_newc_header{
    char c_magic[6]; // The string "070701"
    char c_ino[8];
    char c_mode[8];
    char c_uid[8];
    char c_gid[8];
    char c_nlink[8];
    char c_mtime[8];
    char c_filesize[8];
    char c_devmajor[8];
    char c_devminor[8];
    char c_rdevmajor[8];
    char c_rdevminor[8];
    char c_namesize[8];
    char c_check[8]; // This field	is always set to zero by writers and ignored by	readers.
};

int memcmp(void *s1, void *s2, int n){
    unsigned char *a=s1,*b=s2;
    while(n-->0){ if(*a!=*b) { return *a-*b; } a++; b++; }
    return 0;
}

void memcpy(void *dst, void *src, int n){
    unsigned char *d = dst, *s = src;
    while(n-- > 0){
        *d = *s;
        d++;
        s++;
    }
    d++;
    *d = '\0';
}


void ls(char *addr){
    char *cur = addr ;
    while(1){
        struct cpio_newc_header *cur_header = (struct cpio_newc_header *)cur;
        cur += sizeof(struct cpio_newc_header);
        if(memcmp(cur_header->c_magic, "070701", 6)){
            break;
        }
        char tmp[9];
        tmp[8] = '\0';
        memcpy(tmp, cur_header->c_namesize, 8);
        unsigned int namesize = (unsigned int)htoi(tmp);
        memcpy(tmp, cur_header->c_filesize, 8);
        unsigned int filesize = (unsigned int)htoi(tmp);
        unsigned int adj_namesize = ALIGN(namesize + sizeof(struct cpio_newc_header), 4) - sizeof(struct cpio_newc_header);
        unsigned int adj_filesize = ALIGN(filesize, 4);

        char *filename = cur;
        cur += adj_namesize;
        cur += adj_filesize;
        if(!memcmp(filename, _trailer_, 10)) break;
        uart_send_string(filename);
        uart_send_string("\r\n");
    }
}

void cat(char* addr, char *filename){
    int isFind = 0;
    char *cur = addr ;
    while(1){
        struct cpio_newc_header *cur_header = (struct cpio_newc_header *)cur;
        cur += sizeof(struct cpio_newc_header);
        if(memcmp(cur_header->c_magic, "070701", 6)){
            break;
        }
        char tmp[9];
        tmp[8] = '\0';
        memcpy(tmp, cur_header->c_namesize, 8);
        unsigned int namesize = (unsigned int)htoi(tmp);
        memcpy(tmp, cur_header->c_filesize, 8);
        unsigned int filesize = (unsigned int)htoi(tmp);
        unsigned int adj_namesize = ALIGN(namesize + sizeof(struct cpio_newc_header), 4) - sizeof(struct cpio_newc_header);
        unsigned int adj_filesize = ALIGN(filesize, 4);

        char *curFilename = cur;
        cur += adj_namesize;
        if(!strcmp(curFilename, filename)){
            uart_send_string(cur); // show the file's content
            uart_send_string("\r\n");
            isFind = 1;
            break;
        }
        if(!strcmp(curFilename, _trailer_)){
            break;
        }
        cur += adj_filesize;
    }
    if(!isFind){
        uart_send_string("cat: ");
        uart_send_string(filename);
        uart_send_string(": No such file\r\n");
    }

}

char* ramfs_init(void){
    return (char *)_initramfs_addr_;
}