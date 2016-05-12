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

class Node;

class Edge{

	public:
		Edge(Node *a, Node *b, const int& w);
		Node* node[2];
		int weight;

		Node *getNeighbor(Node *n);

		bool operator < (const Edge& rhs) const;

};

class Node{

	public:
		Node(const int& i);
		void addEdge(Edge *e);
		void sortEdge();

		int id;
		// int color; // 0:white, 1:gray, 2:black
		bool traveled;
		vector<Edge *> edge;

		int d;
		int i;
		int f;
		Node *prev;
		int heap_idx;
};

bool nodeComp (const Node* lhs, const Node* rhs);

class Graph{

	public:
		Graph(const string& n = string("gn"));
		~Graph();
		
		void addEdge(const int& v1, const int& v2, const int& w);
		void sortEdgesOfNode();
		void sortEdgesByWeight();
		void init();
		Node * getNodeById(const int& id);
		int  DFS_Visit(Node* u, int time, ofstream& o);

		map<int, Node *> nodes;
		vector<Edge *> edges;
		string name;
};

#endif
