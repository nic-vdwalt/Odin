#include "box3d/box3d.h"

static unsigned char arena[16 * 1024 * 1024];
static size_t cursor;
static uint64_t ticks;

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

uint64_t b3PlatformTicks( void )
{
	return ticks;
}

int main( void )
{
	b3SetAllocator( testAlloc, testFree );
	uint64_t start = b3GetTicks();
	ticks = 2500;
	if ( b3GetMillisecondsAndReset( &start ) != 2.5f || start != ticks )
	{
		return 1;
	}

	float angle = b3UnwindAngle( 3.0f * B3_PI );
	if ( angle < -B3_PI || angle > B3_PI )
	{
		return 2;
	}

	b3WorldDef worldDef = b3DefaultWorldDef();
	b3WorldId worldId = b3CreateWorld( &worldDef );
	if ( b3World_IsValid( worldId ) == false )
	{
		return 3;
	}
	b3DestroyWorld( worldId );
	return 0;
}
