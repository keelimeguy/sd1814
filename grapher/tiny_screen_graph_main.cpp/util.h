#ifndef UTIL_H
#define UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#define MIN(a, b) (a<b?a:b)
#define MAX(a, b) (a>b?a:b)
#define true 1
#define false 0
#define bool unsigned char

void reverse(char str[], int length);
char* itoa(int num, char* str, int base);
char* ftoa(char *buffer, double d, int precision);

#ifdef __cplusplus
}
#endif

#endif
