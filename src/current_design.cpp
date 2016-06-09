// **************************************************************************
// File       [ current_design.cpp ]
// Author     [ dogiJ ]
// Synopsis   [ ]
// Date       [ 2016/05/23 created ]
// **************************************************************************

#ifndef _CURRENT_DESIGN_CPP_
#define _CURRENT_DESIGN_CPP_

#include "graph.h"
//#include "read_verilog.h"
#include "current_design.h"

bool current_design( Graph *g )
{
    if( g->inputs.empty() ) {
        return false;
    }

    map<string, Node*>::iterator in;
    int count = 0;
    for( in = g->inputs.begin(); in != g->inputs.end(); in++ ) {
        for( int i = 0; i < 2; i++ ) {
            Node *n = in->second;
            g->init();
            n->out_value = i;
            n->solved = true;
            n = path_step( n, count );
//cout << endl;
        }
    }
    cout << "total true path num: " << count << endl;

    /*map<string, bool> input_vector;
    map<string, Node*>::iterator in;
    for( in = g->inputs.begin(); in != g->inputs.end(); in++ ) {
        input_vector[(*in).second->name] = true;
    }

    vector<vector<string> > paths;
    vector<string> path;
    find_true_path( g, path, input_vector );*/

    return true;
}

Node* path_step( Node *n, int &count )
{
    vector<Node*>::iterator i;
    Node *r = n;
//cout << n->name << "(" << n->level << ", " << n->time << ")" << "->";
//if( n->type == OUTPUT ) cout << "output\n";
    if( n->type == OUTPUT ) {
        count++;
        ofstream ofs( "test.out", ofstream::app );
        ofs << endl << count << endl;
        write_true_path( ofs, n, count);
        ofs.close();
    }
    for( i = n->out.begin(); i != n->out.end(); i++ ) {
        Node *o = *i;
        if( !o->solved ) { // not decide the true path yet
            if( !o->determine_path( n ) ) { // non-controlling
                Node *n2 = o->find_in_neighbor(n);
                bool need_value = ( o->type == NAND )? true: false;
                if( n2->level <= n->time ) {
                    vector<Node*> solved_nodes;
                    bool flag = path_back( n2, need_value, solved_nodes );
                    if( flag )
                       r = path_step( o, count );
                    else {
//cout << "fail1\n";
                        vector<Node*>::iterator sn;
                        for( sn = solved_nodes.begin();
                             sn != solved_nodes.end(); sn++ )
                            (*sn)->solved = false;
                    }
                }
                else {
//cout << "fail2\n";
                    o->non_controlling = false;
                }
            }
            else { // find true path
                r = path_step( o, count );
            }
        }
    }
    return r;
}

bool path_back( Node *n, bool need_value, vector<Node*>& solved_nodes )
{
//cout << n->name << "(" << n->level << ", " << n->time << ")" << "<-";
    bool flag = false;
    if( n->solved ) {
        if( n->out_value != need_value )
            flag = false;
        else {
            solved_nodes.push_back( n );
            flag = true;
        }
    }
    else {
        switch( n->type ) {
            case INPUT:
                n->solved = true;
                n->out_value = need_value;
                solved_nodes.push_back( n );
                flag = true;
                break;
            case WIRE:
                n->solved = true;
                n->out_value = need_value;
                solved_nodes.push_back( n );
                flag = path_back( n->in[0], need_value, solved_nodes );
                break;
            case NOT:
                n->solved = true;
                n->out_value = need_value;
                solved_nodes.push_back( n );
                flag = path_back( n->in[0], !need_value, solved_nodes );
                break;
            case NAND:
                n->solved = true;
                n->out_value = need_value;
                solved_nodes.push_back( n );
                if( need_value == false)
                    flag = path_back( n->in[0], true, solved_nodes) &&
                           path_back( n->in[1], true, solved_nodes);
                else {
                    flag = path_back( n->in[0], false, solved_nodes ) ||
                           path_back( n->in[1], false, solved_nodes );
                }
                break;
            case NOR:
                n->solved = true;
                n->out_value = need_value;
                solved_nodes.push_back( n );
                if( need_value == true)
                    flag = path_back( n->in[0], false, solved_nodes) &&
                           path_back( n->in[1], false, solved_nodes);
                else {
                    flag = path_back( n->in[0], true, solved_nodes ) ||
                           path_back( n->in[1], true, solved_nodes );
                }
                break;
            default: break;
        }
    }
    return flag;
}

void write_true_path( ofstream& o, Node *n, int count )
{
    bool flag = true;
    vector<string> lines;
    while( flag ) {
        int i = ( n->true_path-1 <= 0 )? 0: n->true_path-1;
        if( n->type == INPUT ) flag = false;
        Node *prev = ( flag )? n->in[i]: 0;
        if( n->type == WIRE ) {
            n = prev;
            continue;
        }
        stringstream ss;
        char in_value, out_value, in_node;
        switch( n->type ) {
            case NOT:
                in_value = ( prev->out_value )? 'r': 'f';
                out_value = ( n->out_value )? 'r': 'f';
                ss << "  " << n->name << "/A (NOT1)" << " ?" << "0"
                   << "          " << n->time-1 << " " << in_value << endl
                   << "  " << n->name << "/Y (NOT1)" << " ?" << "1"
                   << "          " << n->time << " " << out_value << endl;
                break;
            case NOR:
                in_value = ( prev->out_value )? 'r': 'f';
                out_value = ( n->out_value )? 'r': 'f';
                in_node = ( n->true_path == 1 )? 'A': 'B';
                ss << "  " << n->name << "/"<< in_node << " (NOR2)"
                   << " ?" << "0" << "          " << n->time-1 << " "
                   << in_value << endl
                   << "  " << n->name << "/Y (NOR2)"
                   << " ?" << "1" << "          " << n->time << " "
                   << out_value << endl;
                break;
            case NAND:
                in_value = ( prev->out_value )? 'r': 'f';
                out_value = ( n->out_value )? 'r': 'f';
                in_node = ( n->true_path == 1 )? 'A': 'B';
                ss << "  " << n->name << "/"<< in_node << " (NAND2)"
                   << " ?" << "0" << "          " << n->time-1 << " "
                   << in_value << endl
                   << "  " << n->name << "/Y (NAND2)"
                   << " ?" << "1" << "          " << n->time << " "
                   << out_value << endl;
                break;
            case INPUT:
                out_value = ( n->out_value )? 'r': 'f';
                ss << "  " << n->name << " (in)" << " ?" << "0"
                   << "          " << n->time << " " << out_value << endl;
                break;
            case OUTPUT:
                out_value = ( n->out_value )? 'r': 'f';
                ss << "  " << n->name << " (out)" << " ?" << "0"
                   << "          " << n->time << " " << out_value << endl;
                break;
        }
        lines.push_back( ss.str() );
        n = prev;
    }
    for( vector<string>::iterator i = lines.end()-1;
                                        i != lines.begin(); i--) {
        o << *i;
    }
    o << *lines.begin();
}

#endif
