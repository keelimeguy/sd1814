#ifndef UTIL_H
#define UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#define MIN(a, b) (a<b?a:b)
#define MAX(a, b) (a>b?a:b)
#define bool unsigned char
#define true 1
#define false 0

void reverse(char str[], int length);
#ifndef _STDLIB_H_
char* itoa(int num, char* str, int base);
#endif
char* ftoa(char *buffer, double d, int precision);
//#endif

#ifdef __cplusplus
}
#endif

#endif
