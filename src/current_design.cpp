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
    stringstream ss;
    ss << g->name << ".out";
    ofstream ofs( ss.str().c_str() );
    string case_name = g->name.substr(g->name.find_last_of("/\\") + 1);
    ofs << "Header  {  A True Path Set  }\n\n  Benchmark  {  " << case_name
        << "  }\n\n";
    for( in = g->inputs.begin(); in != g->inputs.end(); in++ ) {
        for( int i = 0; i < 2; i++ ) {
            Node *n = in->second;
            g->init();
            n->out_value = i;
            n->solved = true;
            vector<Node*> solved_nodes;
            n = path_step( n, count, g, solved_nodes, ofs );
//debug
//cout << endl;
//end debug
        }
    }
    cout << "total true path num: " << count << endl;
    ofs.close();

    return true;
}

Node* path_step( Node *n, int &count, Graph *g,
                 vector<Node*>& solved_nodes, ofstream& ofs )
{
    vector<Node*>::iterator i;
    Node *r = n;

    if( n->type == OUTPUT ) {
        count++;
        write_true_path( ofs, g, n, count);
    }
//debug
//cout << n->name << '(' << n->level << ", " <<  n->time << ")->";
//end debug
    for( i = n->out.begin(); i != n->out.end(); i++ ) {
        Node *o = *i;
        if( !o->solved ) { // not decide the true path yet
            solved_nodes.push_back( o );

            if( !o->determine_path( n ) ) { // non-controlling
                Node *n2 = o->find_in_neighbor(n);
                bool need_value = ( o->type == NAND )? true: false;
                size_t pos = solved_nodes.size() - 1;
                if( n2->level <= n->time ) {
                    bool flag = path_back( n2, need_value, solved_nodes );
                    if( flag )
                       r = path_step( o, count, g, solved_nodes, ofs );
                    else {
//debug
//cout << "fail";
//end debug
                        vector<Node*>::iterator sn = solved_nodes.begin() + pos;
                        for( ; sn != solved_nodes.end(); sn++ ) {
                            if( (*sn) != 0 ) (*sn)->solved = false; }
                        solved_nodes.erase( solved_nodes.begin() + pos, sn );
                        o->non_controlling = false;
                        o->time = 0;
                        o->true_path = 0;
                    }
                }
                else {
//debug
//cout << "fail";
//end debug
                    o->non_controlling = false;
                    o->time = 0;
                    o->true_path = 0;
                }
            }
            else { // find true path
                r = path_step( o, count, g, solved_nodes, ofs );
            }
            vector<Node*>::iterator itO = find( solved_nodes.begin(),
                                                solved_nodes.end(), o );
            vector<Node*>::iterator sn;
            for( sn = itO; sn != solved_nodes.end(); sn++ ) {
                Node* t = *(sn);
                t->non_controlling = false;
                t->solved = false;
                t->time = 0;
                t->true_path = 0;
            }
            solved_nodes.erase( itO, solved_nodes.end() );
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

void write_true_path( ofstream& o, Graph *g, Node *n, int count )
{
    o << "  Path  {  " << count << "  }\n\n  A True Path List\n  {\n"
      << "  ---------------------------------------------------------------------------\n"
      << "  Pin    type                                Incr        Path delay\n"
      << "  ---------------------------------------------------------------------------\n";

    bool flag = true;
    vector<string> lines;
    while( flag ) {
//debug
//if( !n->solved ) o << n->name << " fail\n";
//end debug
        int i = ( n->true_path-1 <= 0 )? 0: n->true_path-1;
        if( n->type == INPUT ) flag = false;
        Node *prev = ( flag )? n->in[i]: n;
//debug
//if(n->type !=INPUT && !prev->solved)o<<n->find_in_neighbor(prev)->name << ": "<<n->find_in_neighbor(prev)->solved<<"\n";
//end debug
        char in_value, out_value, in_node;
        stringstream ss;
        if( n->type == WIRE ) {
//debug
                /*in_value = ( prev->out_value )? 'r': 'f';
                out_value = ( n->out_value )? 'r': 'f';
                ss << "  " << n->name << "/A (WIRE1)" << " ?" << "0"
                   << "          " << n->time << " " << in_value << endl
                   << "  " << n->name << "/Y (WIRE1)" << " ?" << "1"
                   << "          " << n->time << " " << out_value << endl;*/
//end debug
            n = prev;
            continue;
        }
        switch( n->type ) {
            case NOT:
                in_value = ( prev->out_value )? 'r': 'f';
                out_value = ( n->out_value )? 'r': 'f';
                ss << "  " << n->name << "/A (NOT1)" << " ?" << "0"
                   << "          " << n->time-n->delay_time << " " << in_value << endl
                   << "  " << n->name << "/Y (NOT1)" << " ?" << "1"
                   << "          " << n->time << " " << out_value << endl;
                break;
            case NOR:
                in_value = ( prev->out_value )? 'r': 'f';
                out_value = ( n->out_value )? 'r': 'f';
                in_node = ( i == 0 )? 'A': 'B';
                ss << "  " << n->name << "/"<< in_node << " (NOR2)"
                   << " ?" << "0" << "          " << n->time-n->delay_time << " "
                   << in_value << endl
                   << "  " << n->name << "/Y (NOR2)"
                   << " ?" << "1" << "          " << n->time << " "
                   << out_value << endl;
                break;
            case NAND:
                in_value = ( prev->out_value )? 'r': 'f';
                out_value = ( n->out_value )? 'r': 'f';
                in_node = ( i == 0 )? 'A': 'B';
                ss << "  " << n->name << "/"<< in_node << " (NAND2)"
                   << " ?" << "0" << "          " << n->time-n->delay_time << " "
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
            default: break;
        }
        lines.push_back( ss.str() );
        n = prev;
    }
    for( vector<string>::iterator i = lines.end()-1;
                                  i != lines.begin(); i--) {
        o << *i;
    }
    o << *lines.begin();

    o << "  ---------------------------------------------------------------------------\n"
      << "  Data Required Time 	        " << 10 << "\n"
      << "  Data Arrival Time            " << 5 << "\n"
      << "  ---------------------------------------------------------------------------\n"
      << "  Slack            	         " << 5 << "\n  }\n\n"
      << "  Input Vector\n  {\n";
    map<string, Node*>::iterator it;
    for( it = g->inputs.begin(); it != g->inputs.end(); it++ ) {
        Node *i = (*it).second;
        char value = ( i->out_value )? '1': '0';
        if( i == n ) value = ( i->out_value )? 'r': 'f';
        o << "    " << i->name << "  =  " << value << endl;
    }
    o << "  }\n\n";
}

#endif
