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
#include <fstream>
#include <iostream>

#define DIS_INF 2147483647

using namespace std;

enum	GateType { INPUT = 0, OUTPUT, NOT, AND, NAND, OR, NOR };

class Node;

class Edge {

	public:
		Edge (string name, const int& id);
		Edge (const int& i) { id = i; }
		Node* node[2];
		string name;
		int time;
		int id;

		Node *getNeighbor (Node *n);
		void addIn (Node *in) { node[0] = in; }
		void addOut (Node *out) { node[1] = out; }
		void setTime (const int& t) { time = t; }
		void addTime (const int& t) { time += t; }

		bool operator < (const Edge& rhs) const;

};

class Node {

	public:
		Node (string n, GateType type);
		void addEdge (Edge *e, int flag);
		// flag = 0:in, 1:out

		string   name;
		GateType type;
		bool     traveled;
		vector<Edge*> in;
		vector<Edge*> out;

		int d;
		int i;
		int f;
		Node *prev;
};


class Graph {

	public:
		Graph(const string& n = string("gn"));
		~Graph();
		
		void addEdge(string name, const int& id);
		void addInput(string name);
		void addOutput(string name);
		void addGate(string name, GateType type,
			     Node *A = 0, Node *Y = 0, Node *B = 0);
		void sortEdgesOfNode();
		void sortEdgesByWeight();
		void init();
		Node * getNode(string name);
		int  DFS_Visit(Node* u, int time, ofstream& o);

		map<string, Node *> inputs;
		map<string, Node *> outputs;
		map<string, Node *> gates;
		map<string, Edge *> wires;
		string name;
};

#endif
