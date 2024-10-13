// SPDX-License-Identifier: MIT

/*
 * Created by lebakassemmerl 2021
 * E-Mail: hotschi@gmx.at
 */

#pragma once

#include <cstddef>
#include <cstdint>

namespace libc {
extern "C" {
    void* memset(void* str, uint8_t val, size_t nr_bytes);
    void* memcpy(void* dst, const void* src, size_t nr_bytes);
    int sprintf(char* buffer, const char* format, ...);
    size_t strlen(const char* str);
}
}