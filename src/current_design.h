// **************************************************************************
// File       [ current_design.h ]
// Author     [ dogiJ ]
// Synopsis   [ ]
// Date       [ 2016/05/23 created ]
// **************************************************************************

#ifndef _CURRENT_DESIGN_H_
#define _CURRENT_DESIGN_H_

#include "graph.h"
//#include "read_verilog.h"

bool current_design( Graph *g );

Node* path_step( Node *n, int &count );

bool path_back( Node *n, bool need_value, vector<Node*>& solved_nodes );

void write_true_path( ofstream& o, Node *n, int count );

#endif

