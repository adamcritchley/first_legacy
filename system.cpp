#ifndef WIN32

#include "system.h"
#include "iostream.h"
#include "string.h"
#include <stdio.h>

void flsystem( char *szString )
{
	if( !strcmpi( "cls", szString ) )
	{
		system( "clear" );
	}
	else if( !strcmpi( "pause", szString ) )
	{
		cout << "Press any key to continue..."
			 << endl;
		cin.get();
	}
}

#endif