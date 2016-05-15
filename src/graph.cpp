// **************************************************************************
// File       [ graph.cpp ]
// Author     [ dogiJ ]
// Synopsis   [ ]
// Date       [ 2016/05/14 created ]
// **************************************************************************
#include "graph.h"

Edge::Edge(string name, const int& id)
{
	this->name = name;
	this->id = id;
}

bool Edge::operator < (const Edge& rhs) const{

}
Node * Edge::getNeighbor(Node *n)
{
	if ( node[0] == n ) return node[1];
	if ( node[1] == n ) return node[0];

	return 0;
}


Node::Node (string n, GateType type)
{
	name = n;
	type = type;
	traveled = false;
	d = DIS_INF;
	prev = 0;
}

void Node::addEdge (Edge *e, int flag)
{
	if(flag == 0) {
		
	}
	else {
		
	}
}

Graph::Graph(const string& n)
{
	name = n;
}

Graph::~Graph()
{
	map<string, Edge *>::iterator itE;
	for ( itE = wires.begin() ; itE != wires.end() ; itE++ )
	{
		delete (*itE).second;
		(*itE).second = 0;
	}
	
	map<string, Node *>::iterator itN;
	for ( itN = gates.begin() ; itN != gates.end() ; itN++ )
	{
		delete (*itN).second;
		(*itN).second = 0;
	}
	for ( itN = inputs.begin() ; itN != inputs.end() ; itN++ )
	{
		delete (*itN).second;
		(*itN).second = 0;
	}
	for ( itN = outputs.begin() ; itN != outputs.end() ; itN++ )
	{
		delete (*itN).second;
		(*itN).second = 0;
	}
}
		
void Graph::addEdge(string name, const int& id)
{
	Edge *e = new Edge(name, id);
	wires[name] = e;
}

void Graph::addInput(string name)
{
	Node *n = new Node(name, GateType(INPUT));
	inputs[name] = n;
}

void Graph::addOutput(string name)
{
	Node *n = new Node(name, GateType(OUTPUT));
	outputs[name] = n;
}

void Graph::addGate(string name, GateType type, Node *A, Node *Y, Node *B )
{
	Node *n;
	switch(type) {
		case NOT:
			n = new Node(name, type);
			gates[name] = n;
			break;
		case AND:
			n = new Node(name, type);
			gates[name] = n;
			break;
		case NAND:
			n = new Node(name, type);
			gates[name] = n;
			break;
		case OR:
			n = new Node(name, type);
			gates[name] = n;
			break;
		case NOR:
			n = new Node(name, type);
			gates[name] = n;
			break;
	}
}
	
void Graph::sortEdgesOfNode()
{

}

bool edgeCompByW( const Edge* A, const Edge* B ){
	return false;
}

void Graph::sortEdgesByWeight()
{
}
void Graph::init()
{
	map<string, Node *>::iterator itN;
	for ( itN = gates.begin() ; itN != gates.end() ; itN++ )
	{
		Node *node = (*itN).second;
		node->traveled = false;
		node->d = DIS_INF;
		node->i = 0;
		node->f = 0;
		node->prev = 0;
	}
	
}

Node * Graph::getNode(string name)
{
	return gates[name];
}

/*int Graph::DFS_Visit (Node* u, int time, ofstream& o)
{
	time++;
	u->i = time;
	u->traveled = true;
	for (vector<Edge*>::iterator i = u->edge.begin();
		i != u->edge.end(); ++i) {
		Node* v = (*i)->getNeighbor(u);
		if (v->traveled == false) {
			o << "v" << u->id << " -- v" << v->id
			  << " [label = \"" << (*i)->weight << "\"];\n";
			v->prev = u;
			time = Graph::DFS_Visit(v, time, o);
		}
	}
	time++;
	u->f = time;
	return time;
}*/
