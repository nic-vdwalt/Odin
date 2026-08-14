#pragma once

#include <float.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef PRIx64
#define PRIx64 "llx"
#endif

#ifndef PRIu64
#define PRIu64 "llu"
#endif

#define fabsf( value ) __builtin_fabsf( value )
#define floorf( value ) __builtin_floorf( value )
#define isfinite( value ) __builtin_isfinite( value )
#define isinf( value ) __builtin_isinf( value )
#define isnan( value ) __builtin_isnan( value )

void* memcpy( void* destination, const void* source, size_t size );
void* memmove( void* destination, const void* source, size_t size );
void* memset( void* destination, int value, size_t size );
int memcmp( const void* left, const void* right, size_t size );
char* strncpy( char* destination, const char* source, size_t size );
int strcmp( const char* left, const char* right );
size_t strlen( const char* string );
void qsort( void* base, size_t count, size_t size, int ( *compare )( const void*, const void* ) );
