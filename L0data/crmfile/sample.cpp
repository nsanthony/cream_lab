//
// sample.cpp
//
// 2004/4/15 SYZ
//
// Copyright (C) 2003-2004 by the CREAM project.  All rights reserved.
// 
// This file is part of CDAQ, the CREAM data acquisition and processing system.
// 
// This work may not be reproduced, displayed, modified or distributed
// without the express prior written permission of the copyright holder.
// For permission, contact Prof. Eun-suk Seo at seo@umd.edu.  The above
// copyright notice must appear in all copies and in supporting documentation.
// 
// The software is provided without any warranty, express or implied, including
// but not limited to the warranties of merchantability, fitness for a
// particular purpose and noninfringement.  In no event shall the author or
// the CREAM project group be liable for any claim, damages or other liability,
// whether in an action of contract, tort or otherwise, arising from, out of
// or in connection with the software or the use or other dealings in the
// software.
//
#include "CreamFile.h"

void print_out(CreamFile& cfile, FILE* fp)
	{
	unsigned nevent;
	DEvent ev;

	cfile.PrintRunHeader( fp );

	for(nevent = 1; ; nevent++) {
		/* clear event data */
		ev.Clear();

		/* read an event */
		if ( cfile.ReadEvent( ev ) == -1 ) break;

		fprintf(fp, "========== EVENT %d ==========\n", nevent);
		ev.Print( fp );
	}

	cfile.PrintRunTail( fp );
}

void do_file(char* filename)
	{
	CreamFile cfile;

	if ( cfile.Open( filename ) == 0 ) {
		print_out( cfile, stdout );
		cfile.Close();
	}
	else
		fprintf(stderr, "ERROR: cannot open file %s\n", filename);
}

int main(int argc, char** argv)
	{
	int k;

	for(k = 1; k < argc; k++)
		do_file( argv[k] );
	
	return 0;
}