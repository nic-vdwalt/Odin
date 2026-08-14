#include "wasm_compat.h"

#include <stdio.h>

FILE* fopen( const char* path, const char* mode )
{
	(void)path;
	(void)mode;
	return NULL;
}

int fclose( FILE* stream )
{
	(void)stream;
	return EOF;
}

int fseek( FILE* stream, long offset, int origin )
{
	(void)stream;
	(void)offset;
	(void)origin;
	return -1;
}

long ftell( FILE* stream )
{
	(void)stream;
	return -1;
}

size_t fread( void* destination, size_t size, size_t count, FILE* stream )
{
	(void)destination;
	(void)size;
	(void)count;
	(void)stream;
	return 0;
}

size_t fwrite( const void* source, size_t size, size_t count, FILE* stream )
{
	(void)source;
	(void)size;
	(void)count;
	(void)stream;
	return 0;
}

int fscanf( FILE* stream, const char* format, ... )
{
	(void)stream;
	(void)format;
	return EOF;
}

int vfprintf( FILE* stream, const char* format, va_list args )
{
	(void)stream;
	(void)format;
	(void)args;
	return 0;
}

int vsnprintf( char* buffer, size_t size, const char* format, va_list args )
{
	(void)format;
	(void)args;
	if ( size > 0 )
	{
		buffer[0] = '\0';
	}
	return 0;
}

void* memcpy( void* destination, const void* source, size_t size )
{
	unsigned char* dst = destination;
	const unsigned char* src = source;
	for ( size_t i = 0; i < size; ++i )
	{
		dst[i] = src[i];
	}
	return destination;
}

void* memmove( void* destination, const void* source, size_t size )
{
	unsigned char* dst = destination;
	const unsigned char* src = source;
	if ( dst < src )
	{
		for ( size_t i = 0; i < size; ++i )
		{
			dst[i] = src[i];
		}
	}
	else if ( dst > src )
	{
		for ( size_t i = size; i > 0; --i )
		{
			dst[i - 1] = src[i - 1];
		}
	}
	return destination;
}

void* memset( void* destination, int value, size_t size )
{
	unsigned char* dst = destination;
	for ( size_t i = 0; i < size; ++i )
	{
		dst[i] = (unsigned char)value;
	}
	return destination;
}

int memcmp( const void* left, const void* right, size_t size )
{
	const unsigned char* a = left;
	const unsigned char* b = right;
	for ( size_t i = 0; i < size; ++i )
	{
		if ( a[i] != b[i] )
		{
			return (int)a[i] - (int)b[i];
		}
	}
	return 0;
}

char* strncpy( char* destination, const char* source, size_t size )
{
	size_t i = 0;
	for ( ; i < size && source[i] != '\0'; ++i )
	{
		destination[i] = source[i];
	}
	for ( ; i < size; ++i )
	{
		destination[i] = '\0';
	}
	return destination;
}

int strcmp( const char* left, const char* right )
{
	while ( *left != '\0' && *left == *right )
	{
		left += 1;
		right += 1;
	}
	return (int)(unsigned char)*left - (int)(unsigned char)*right;
}

size_t strlen( const char* string )
{
	size_t length = 0;
	while ( string[length] != '\0' )
	{
		length += 1;
	}
	return length;
}

static void b3SwapBytes( unsigned char* left, unsigned char* right, size_t size )
{
	for ( size_t i = 0; i < size; ++i )
	{
		unsigned char value = left[i];
		left[i] = right[i];
		right[i] = value;
	}
}

void qsort( void* base, size_t count, size_t size, int ( *compare )( const void*, const void* ) )
{
	unsigned char* bytes = base;
	for ( size_t i = 1; i < count; ++i )
	{
		size_t j = i;
		while ( j > 0 && compare( bytes + j * size, bytes + ( j - 1 ) * size ) < 0 )
		{
			b3SwapBytes( bytes + j * size, bytes + ( j - 1 ) * size, size );
			j -= 1;
		}
	}
}
