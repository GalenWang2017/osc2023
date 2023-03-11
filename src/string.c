#include "string.h"

int strcmp(const char *str1, const char *str2)
{
    char c1, c2;

    while ((c1 = *str1++) == (c2 = *str2++) && c1 && c2) {};

    return c1 - c2;
}

int strncmp(const char *str1, const char *str2, int n)
{
    char c1, c2;

    if (n <= 0)
        return 0;

    while ((c1 = *str1++) == (c2 = *str2++) && c1 && c2 && --n > 0) {};

    return n ? c1 - c2 : 0;
}

int strcasecmp(const char *s1, const char *s2)
{
    char c1, c2;

    while (1) {
        c1 = *s1++;
        c2 = *s2++;

        if (!c1 || !c2) {
            break;
        }

        if ('A' <= c1 && c1 <= 'Z') {
            c1 |= 0x20;
        }

        if ('A' <= c2 && c2 <= 'Z') {
            c2 |= 0x20;
        }

        if (c1 != c2) {
            break;
        }
    }

    return c1 - c2;
}

int strlen(const char *str)
{
    int ret = 0;
    
    while (*str++) {
        ret += 1;
    }

    return ret;
}

int strcpy(char *dst, const char *src){
    int ret = 0;
    while (*src) {
        *dst = *src;
        dst++;
        src++;
        ret++;
    }

    *dst = '\0';

    return ret;
}

char *strcat(char *dest, const char *src)
{
    char *t;

    t = dest;

    while (*t) {
        t++;
    }

    while (*src) {
        *t = *src;
        t++;
        src++;
    }

    *t = '\0';

    return dest;
}

char *strncat(char *dest, const char *src, int n)
{
    char *t;

    t = dest;

    while (*t) {
        t++;
    }

    while (n > 0 && *src) {
        *t = *src;
        t++;
        src++;
        n--;
    }

    *t = '\0';

    return dest;
}

int atoi(const char *str)
{
    int i = 0, tmp = 0;

    while (*str) {
        if ('0' > *str || *str > '9') {
            return 0;
        }

        i *= 10;

        tmp = i + (*str - '0');

        if (tmp < i) {
            // Maybe overflow
            return 0;
        }

        i = tmp;

        str++;
    }

    return i;
}

int htoi(const char *str){
    /*
        hex string convert to decimal int (positive only)
    */
    int rlt = 0;
    while(*str){
        if(*str >= 'A' && *str <= 'F'){
            rlt = rlt + (*str - 'A' + 10);
        }else if(*str >= '0' && *str <= '9'){
            rlt = rlt + (*str - '0');
        }else if(*str >= 'a' && *str <= 'a'){
            rlt = rlt + (*str - 'a' + 10);
        }
        rlt = rlt << 4;
        str++;
    }
    return rlt >> 4;
}