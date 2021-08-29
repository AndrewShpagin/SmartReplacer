#include "comms-Math.h"

namespace comms {

int cList_Debug::ContainersCount = 0;
int cList_Debug::CtorContainersCount = 0;

} // comms

// @test
#if 0
#include "stdafx.h"
#include <iostream>
#include <fstream>
void doCListAndExit() {
	// redirect output to the file
	const comms::cStr  testFile = "test.doCListAndExit.log";
	FILE*  file = freopen( testFile.ToCharPtr(), "w", stdout );
	assert( file );

	// test output
	{
		cList< int >  l;
		l.Add( -3 );
		l.Add( -2 );
		l.Add( 10 );
		l.Add( -2 );
		l.Add( 30 );
		std::cout << l << "\n\n";
	}

	{
		typedef comms::cVec< int, 4 >  node_t;
		cList< node_t >  l;
		l.Add( node_t( 0, 1, 2, 3 ) );
		l.Add( node_t( 4, 5, 6, 7 ) );
		l.Add( node_t( 0, 1, 2, 3 ) );
		l.Add( node_t( 40, -20, -10, -5 ) );
		l.Add( node_t( 100, 200, 300, 400 ) );
		std::cout << l << "\n\n";
	}

	exit( 0 );
}

CallItLater  startCList( &doCListAndExit, BASIC_TOOL );
#endif
