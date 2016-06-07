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
    if( n->type == OUTPUT ) count++;
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
                    if( n->in[0]->level <= n->in[1]->level )
                        flag = path_back( n->in[0], false, solved_nodes );
                    else
                        flag = path_back( n->in[1], false, solved_nodes );
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
                    if( n->in[0]->level <= n->in[1]->level )
                        flag = path_back( n->in[0], true, solved_nodes );
                    else
                        flag = path_back( n->in[1], true, solved_nodes );
                }
                break;
            default: break;
        }
    }
    return flag;
}

/*void find_true_path( Graph *g, vector<string>& path,
                     map<string, bool>& input_vector )
{

    g->init();
    map<string, Node*>::iterator in;
    for( in = g->inputs.begin(); in != g->inputs.end(); in++ ) {
        in->second->out_value = input_vector[in->second->name];
    }
    for( in = g->inputs.begin(); in != g->inputs.end(); in++ ) {
        Node *n = in->second;
        while( n->out.size() != 0 ) {
            vector<Node*>::iterator it;
            for( it = n->out.begin();
                 it != n->out.end(); it++ ) {
                                
            }
        }
    }

}

void go_path( Node *n )
{
    vector<Node*>::iterator out;
    for( out = n->out.begin(); out != n->out.end(); out++ ) {
        
        go_path( *out );
    }
}*/

#endif
