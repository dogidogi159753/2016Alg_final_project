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
    int i;
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
                            i = strcspn( tok2, digits );
                            g->addInput( string(tok2) );
                            break;
                        case 2: // output
                            i = strcspn( tok2, digits );
                            g->addOutput( string(tok2) );
                            break;
                        case 3: // wire
                            i = strcspn( tok2, digits );
                            g->addEdge( string(tok2), atoi(&tok2[i]) );
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
            tok2 = strtok( NULL, " (" );
            tok2 = strtok( NULL, ")" );
            if( t == NOT) {
                string tmp(tok2);
                if( g->inputs.find(tmp) != g->inputs.end() ) {
                    g->inputs[tmp]->out.push_back( gt );
                    gt->in.push_back( g->inputs[tmp] );
                }
                else if( g->outputs.find(tmp) != g->outputs.end() ) {
                    g->outputs[tmp]->out.push_back( gt );
                    gt->in.push_back( g->outputs[tmp] );
                }
                else {
                    g->wires[tmp]->out.push_back( gt );
                    gt->in.push_back( g->wires[tmp] );
                }
                tok2 = strtok( NULL, "(" );
                tok2 = strtok( NULL, ")" );
                tmp = string(tok2);
                if( g->outputs.find(tmp) != g->outputs.end() ) {
                    g->outputs[tmp]->in.push_back( gt );
                    gt->out.push_back( g->outputs[tmp] );
                }
                else {
                    g->wires[tmp]->in.push_back( gt );
                    gt->out.push_back( g->wires[tmp] );
                }
            }
            else {
                string tmp(tok2);
                if( g->inputs.find(tmp) != g->inputs.end() ) {
                    g->inputs[tmp]->out.push_back( gt );
                    gt->in.push_back( g->inputs[tmp] );
                }
                else if( g->outputs.find(tmp) != g->outputs.end() ) {
                    g->outputs[tmp]->out.push_back( gt );
                    gt->in.push_back( g->outputs[tmp] );
                }
                else {
                    g->wires[tmp]->out.push_back( gt );
                    gt->in.push_back( g->wires[tmp] );
                }
                tok2 = strtok( NULL, "(" );
                tok2 = strtok( NULL, ")" );
                tmp = string(tok2);
                if( g->inputs.find(tmp) != g->inputs.end() ) {
                    g->inputs[tmp]->out.push_back( gt );
                    gt->in.push_back( g->inputs[tmp] );
                }
                else if( g->outputs.find(tmp) != g->outputs.end() ) {
                    g->outputs[tmp]->out.push_back( gt );
                    gt->in.push_back( g->outputs[tmp] );
                }
                else {
                    g->wires[tmp]->out.push_back( gt );
                    gt->in.push_back( g->wires[tmp] );
                }
                tok2 = strtok( NULL, "(" );
                tok2 = strtok( NULL, ")" );
                tmp = string(tok2);
                if( g->outputs.find(tmp) != g->outputs.end() ) {
                    g->outputs[tmp]->in.push_back( gt );
                    gt->out.push_back( g->outputs[tmp] );
                }
                else {
                    g->wires[tmp]->in.push_back( gt );
                    gt->out.push_back( g->wires[tmp] );
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

    f.close();
    return true;
}

