#include "util.h"

#ifdef __cplusplus
extern "C" {
#endif

// http://www.geeksforgeeks.org/implement-itoa/

/* A utility function to reverse a string  */
void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    char temp;
    while (start < end)
    {
        temp = *(str+start);
        *(str+start) = *(str+end);
        *(str+end) = temp;
        start++;
        end--;
    }
}

// Implementation of itoa()
char* itoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }

    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    return str;
}

// https://stackoverflow.com/questions/16647278/minimal-implementation-of-sprintf-or-printf

char *ftoa(char *buffer, double d, int precision) {

  long wholePart = (long) d;

  // Deposit the whole part of the number.

  itoa(wholePart,buffer,10);

  // Now work on the faction if we need one.

  if (precision > 0) {

    // We do, so locate the end of the string and insert
    // a decimal point.

    char *endOfString = buffer;
    while (*endOfString != '\0') endOfString++;
    *endOfString++ = '.';

    // Now work on the fraction, be sure to turn any negative
    // values positive.

    if (d < 0) {
      d *= -1;
      wholePart *= -1;
    }

    double fraction = d - wholePart;
    while (precision > 0) {

      // Multipleby ten and pull out the digit.

      fraction *= 10;
      wholePart = (long) fraction;
      *endOfString++ = '0' + wholePart;

      // Update the fraction and move on to the
      // next digit.

      fraction -= wholePart;
      precision--;
    }

    // Terminate the string.

    *endOfString = '\0';
  }

   return buffer;
}

#ifdef __cplusplus
}
#endif
