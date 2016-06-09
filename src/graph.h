// **************************************************************************
// File       [ graph.h ]
// Author     [ dogiJ ]
// Synopsis   [ ]
// Date       [ 2016/05/14 created ]
// **************************************************************************
#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>

#define DIS_INF 2147483647

using namespace std;

enum	GateType { INPUT = 0, OUTPUT, WIRE, NOT, AND, NAND, OR, NOR };

class Node;

class Edge {

	public:
		Edge (string name);
		Node* node[2];
		string name;
		int time;

		Node *getNeighbor (Node *n);
		void addIn (Node *in) { node[0] = in; }
		void addOut (Node *out) { node[1] = out; }

		bool operator < (const Edge& rhs) const;

};

class Node {

	public:
		Node (string n, GateType t);
		// void addEdge (Edge *e, int flag);
		// flag = 0:in, 1:out

		string   name;
		GateType type;

		bool non_controlling;
		bool traveled;
		bool out_value;
		bool solved;
		int  true_path; // 0:no, 1:in[0], 2:in[1]
		int  level;

		int time; // 0:out, 1:in1, 2:in2
		int delay_time;

		vector<Node*> in;
		vector<Node*> out;

		int d;
		int f;
		Node *prev;

		size_t find_in(Node *n);
		Node*  find_in_neighbor(Node *n);
		bool   determine_path(Node *n);
};


class Graph {

	public:
		Graph(const string& n = string("case"));
		~Graph();
		
		void addWire(string name);
		void addInput(string name);
		void addOutput(string name);
		void addGate(string name, GateType type);
		// void sortEdgesOfNode();
		// void sortEdgesByWeight();
		void init();
		Node * getNode(string name);
		int  DFS_Visit(Node* u, int time);

		map<string, Node *> inputs;
		map<string, Node *> outputs;
		map<string, Node *> gates;
		map<string, Node *> wires;
		string name;
};

#endif
