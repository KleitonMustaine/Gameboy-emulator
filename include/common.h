#pragma once

#define COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define BIT(a, n) ((a) & (1 << (n)) ? 1 : 0) 
#define BIT_SET(a, n, on) do { if (on) (a) |= (1 << (n)); else (a) &= ~(1 << (n)); } while (0)
#define BETWEEN(a, b, c) ((a >= b) && (a <= c))

void delay(u32 ms);

#define NO_IMPL { fflush(stdout); \
    fprintf(stderr, "NOT IMPLEMENTED YET (%s:%d)\n", __FILE__, __LINE__); \
    exit (-5);}



