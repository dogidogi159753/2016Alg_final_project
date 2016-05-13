// **************************************************************************
// File       [ read_verilog.cpp ]
// Author     [ dogiJ ]
// Synopsis   [ ]
// Date       [ 2016/05/13 created ]
// **************************************************************************

#include "read_verilog.h"
#include <vector>
#include <cstring>
#include <iostream>
#include <fstream>
using namespace std;

bool read_verilog( const char* name ) {

    ifstream f(name);
    if( !f.is_open() ) {
        fprintf( stderr, "**ERROR read_verilog: " );
        fprintf( stderr, "no such file\n" );
        return false;
    }

    f.seekg( 0, ifstream::end );
    int length = f.tellg();
    cout << "This file has " << length << " chars\n";
    f.seekg( 0, ifstream::beg );

    char* buffer = new char[length];
    f.read( buffer, length );
    cout << buffer;

    delete[] buffer;
    f.close();
    return true;
}

