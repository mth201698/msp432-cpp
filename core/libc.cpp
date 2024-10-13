// SPDX-License-Identifier: MIT

/*
 * Created by lebakassemmerl 2024
 * E-Mail: hotschi@gmx.at
 */

#include <cstddef>
#include <cstdint>
#include <cstdarg>

#include "libc.h"

extern void hard_fault(void);

namespace libc {
void itoa(int value, char* buffer, int base) {
    char* ptr = buffer;
    char* end = buffer;
    bool isNegative = (value < 0 && base == 10);

    if (isNegative) value = -value;

    // Convert value to string (reverse order).
    do {
        int digit = value % base;
        *end++ = (digit < 10) ? '0' + digit : 'A' + (digit - 10);
        value /= base;
    } while (value);

    if (isNegative) *end++ = '-';

    // Reverse the string.
    *end = '\0';
    while (ptr < --end) {
        char temp = *ptr;
        *ptr++ = *end;
        *end = temp;
    }
}

extern "C" {
    size_t strlen(const char* str)
    {
        size_t len = 0;

        while (*str) {
            str++;
            len++;
        }

        return len;
    }

    int sprintf(char* buffer, const char* format, ...) {
        va_list args;
        va_start(args, format);

        char* bufPtr = buffer;

        for (const char* fmt = format; *fmt != '\0'; ++fmt) {
            if (*fmt == '%') {
                ++fmt;  // Move past '%'
                switch (*fmt) {
                    case 'd': {  // Integer
                        int value = va_arg(args, int);
                        char temp[12];  // Enough to hold an int (-2147483648 to 2147483647).
                        itoa(value, temp, 10);
                        for (char* t = temp; *t != '\0'; ++t) {
                            *bufPtr++ = *t;
                        }
                        break;
                    }
                    case 'l': {  // Long integer
                        if (*(fmt + 1) == 'u') {
                            ++fmt;  // Move past 'u'
                            unsigned long value = va_arg(args, unsigned long);
                            char temp[21];  // Enough to hold any long unsigned int
                            itoa(value, temp, 10);
                            for (char* t = temp; *t != '\0'; ++t) {
                                *bufPtr++ = *t;
                            }
                        }
                        break;
                    }
                    case 'x': {  // Hexadecimal
                        int value = va_arg(args, int);
                        char temp[12];
                        itoa(value, temp, 16);
                        for (char* t = temp; *t != '\0'; ++t) {
                            *bufPtr++ = *t;
                        }
                        break;
                    }
                    case 's': {  // String
                        char* str = va_arg(args, char*);
                        while (*str) {
                            *bufPtr++ = *str++;
                        }
                        break;
                    }
                    default:  // Unsupported specifier
                        *bufPtr++ = '%';
                        *bufPtr++ = *fmt;
                        break;
                }
            } else {
                *bufPtr++ = *fmt;  // Copy regular characters
            }
        }

        *bufPtr = '\0';  // Null-terminate the buffer
        va_end(args);

        return bufPtr - buffer;  // Return the number of characters written
    }
    }
}