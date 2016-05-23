// **************************************************************************
// File       [ graph.cpp ]
// Author     [ dogiJ ]
// Synopsis   [ ]
// Date       [ 2016/05/14 created ]
// **************************************************************************
#include "graph.h"

Edge::Edge(string name)
{
	this->name = name;
}

bool Edge::operator < (const Edge& rhs) const
{
	return this->time < rhs.time;
}
Node * Edge::getNeighbor(Node *n)
{
	if ( node[0] == n ) return node[1];
	if ( node[1] == n ) return node[0];

	return 0;
}


Node::Node (string n, GateType t)
{
	name = n;
	type = t;
	true_path = 2;
	traveled = false;
	time = 0;
	delay_time = 1;
	prev = 0;
}

/*void Node::addEdge (Edge *e, int flag)
{
	if(flag == 0) {
		
	}
	else {
		
	}
}*/

Graph::Graph(const string& n)
{
	name = n;
}

Graph::~Graph()
{
	map<string, Node *>::iterator itN;
	for ( itN = wires.begin() ; itN != wires.end() ; itN++ )
	{
		delete (*itN).second;
		(*itN).second = 0;
	}
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
		
void Graph::addWire(string name)
{
	if(wires.find(name) == wires.end())
	{
		Node *e = new Node(name, GateType(WIRE));
		wires[name] = e;
		e->delay_time = 0;
	}
}

void Graph::addInput(string name)
{
	if(inputs.find(name) == inputs.end())
	{
		Node *n = new Node(name, GateType(INPUT));
		inputs[name] = n;
		n->delay_time = 0;
	}
}

void Graph::addOutput(string name)
{
	if(outputs.find(name) == outputs.end())
	{
		Node *n = new Node(name, GateType(OUTPUT));
		outputs[name] = n;
		n->delay_time = 0;
	}
}

void Graph::addGate(string name, GateType type)
{
	if(gates.find(name) == gates.end())
	{
		Node *n = new Node(name, type);
		gates[name] = n;
	}
}
	
/*void Graph::sortEdgesOfNode()
{

}

bool edgeCompByW( const Edge* A, const Edge* B ){
	return false;
}

void Graph::sortEdgesByWeight()
{
}*/
void Graph::init()
{
	map<string, Node *>::iterator it;
	for ( it = gates.begin(); it != gates.end(); it++ )
	{
		Node *node = (*it).second;
		node->traveled = false;
		node->prev = 0;
	}
	
}

Node * Graph::getNode(string name)
{
	if(inputs.find(name) != inputs.end())
		return inputs[name];
	if(outputs.find(name) != outputs.end())
		return outputs[name];
	if(wires.find(name) != wires.end())
		return wires[name];
	if(gates.find(name) != gates.end())
		return gates[name];
	return NULL;
}

int Graph::DFS_Visit (Node* u, int time)
{
	time++;
	u->d = time;
	u->traveled = true;
	vector<Node*>::iterator itN;
	for(itN = u->out.begin(); itN != u->out.end(); itN++) {
		//if((*itN)->traveled == false) {
		if((*itN)->time <= u->time) {
			(*itN)->prev = u;
			(*itN)->time = u->time + u->delay_time;
			time = this->DFS_Visit((*itN), time);
		}
	}
	time++;
	u->f = time;
        return time;
}
