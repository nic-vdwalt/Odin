#include "box3d/base.h"

#include <stddef.h>
#include <stdint.h>

#define B3_EXPORT( name ) __attribute__( ( export_name( name ) ) )

typedef struct b3Mutex b3Mutex;
b3Mutex* b3CreateMutex( void );
void b3LockMutex( b3Mutex* mutex );
void b3UnlockMutex( b3Mutex* mutex );

static unsigned char arena[1024 * 1024];
static size_t cursor;
static b3Mutex* mutex;
static uint32_t value;

static void* testAlloc( int32_t size, int32_t alignment )
{
	uintptr_t start = ( (uintptr_t)( arena + cursor ) + (uintptr_t)alignment - 1 ) & ~( (uintptr_t)alignment - 1 );
	cursor = (size_t)( start - (uintptr_t)arena ) + (size_t)size;
	return cursor <= sizeof( arena ) ? (void*)start : NULL;
}

static void testFree( void* memory )
{
	(void)memory;
}

B3_EXPORT( "box3d_mutex_test_init" )
void box3dMutexTestInit( void )
{
	b3SetAllocator( testAlloc, testFree );
	mutex = b3CreateMutex();
	value = 0;
}

B3_EXPORT( "box3d_mutex_test_run" )
void box3dMutexTestRun( uint32_t count )
{
	for ( uint32_t i = 0; i < count; ++i )
	{
		b3LockMutex( mutex );
		value += 1;
		b3UnlockMutex( mutex );
	}
}

B3_EXPORT( "box3d_mutex_test_value" )
uint32_t box3dMutexTestValue( void )
{
	return value;
}
