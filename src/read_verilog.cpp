// **************************************************************************
// File       [ read_verilog.cpp ]
// Author     [ dogiJ ]
// Synopsis   [ ]
// Date       [ 2016/05/13 created ]
// **************************************************************************

#include "read_verilog.h"
#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
using namespace std;

// need to fix: only NAND and NOR; position of Y, A, B is not fixed

bool read_verilog( const char* name, Graph* g ) {

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

    char buffer[length];
    f.read( buffer, length );
    char *str = buffer;
    char *tok = strchr( str, ';' );
    char digits[] = "0123456789";
    int flag = 0;
    while( tok != NULL ) {
        *tok++ = '\0';
        if( flag != 3 ) {
            char* tok2 = strtok( str, ", \n");
            while( tok2 != NULL ) {
                if( flag == 0 && string(tok2) == string("module") ) {
                    tok2 = strtok( NULL, ", \n");
                    g->name = string(tok2);
                    break;
                }
                else if( flag == 0 && string(tok2) == string("input") ) {
                    flag++;
                }
                else if( flag == 1 && string(tok2) == string("output") ) {
                    flag++;
                }
                else if( flag == 2 && string(tok2) == string("wire") ) {
                    flag++;
                }
                else {
                    switch( flag ) {
                        case 1: // input
                            g->addInput( string(tok2) );
                            break;
                        case 2: // output
                            g->addOutput( string(tok2) );
                            break;
                        case 3: // wire
                            g->addWire( string(tok2) );
                            break;
                        default: break;
                    }
                }
                tok2 = strtok( NULL, ", \n");
            }
        }
        else {
            char* tok2 = strtok( str, "\n " );
            int len = strcspn( tok2, digits );
            char tmp[len+1];
            strncpy( tmp, tok2, len);
            tmp[len] = '\0';
            GateType t;
            if( string(tmp) == string("NOT") ) t = NOT;
            else if( string(tmp) == string("AND") ) t = AND;
            else if( string(tmp) == string("NAND") ) t = NAND;
            else if( string(tmp) == string("OR") ) t = OR;
            else if( string(tmp) == string("NOR") ) t = NOR;

            tok2 = strtok( NULL, " ");
            string name(tok2);
            g->addGate( name, t);
            Node *gt = g->gates[name];
            int gn = ( t == NOT)? 2: 3;
            for( int i = 0; i < gn; i++ ) {
                tok2 = strtok( NULL, "." );
                tok2 = strtok( NULL, "(" );
                bool flag = 1;// 0:Y, 1:A or B
                if( string(tok2) == string("Y"))
                    flag = 0;
                else flag = 1;
                tok2 = strtok( NULL, ")" );
                string tmp(tok2);
                Node *n = g->getNode( tmp );
                if( flag == 0) {
                    n->in.push_back(gt);
                    gt->out.push_back(n);
                }
                else {
                    n->out.push_back(gt);
                    gt->in.push_back(n);
                }
            }
        }
        str = tok;
        tok = strchr( str, ';' );
    }

    cout << "check graph size:\ninput: " << g->inputs.size()
         << "\noutput: " << g->outputs.size()
         << "\nwire: " << g->wires.size()
         << "\ngate: " << g->gates.size() << endl;

    /*for( map<string, Node*>::iterator i = g->gates.begin();
                                      i != g->gates.end(); i++ ) {
        Node *n = (*i).second;
        cout << "gate " << n->name << ":\n"
             << "type: " << n->type << endl
             << "in: " << n->in[0]->name << endl;
        if( n->type != NOT)
            cout << "in2:" << n->in[1]->name << endl;
        cout << "out: " << n->out[0]->name << endl;
    }*/

    f.close();

    // build dfs
    map<string, Node*>::iterator it;
    for( it = g->inputs.begin(); it != g->inputs.end(); it++ ) {
        int time = 0;
        time = g->DFS_Visit(it->second, time);
    }
    /*for( it = g->outputs.begin(); it != g->outputs.end(); it++) {
        Node *n = it->second;
        cout << "name: " << n->name
             << ", time: " << n->time
             << ", level: " << n->level << "\n\n";
    }
    for( it = g->gates.begin(); it != g->gates.end(); it++) {
        Node *n = it->second;
        cout << "name: " << n->name
             << ", time: " << n->time
             << ", level: " << n->level << "\n\n";
    }*/

    return true;
}

