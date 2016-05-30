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
	true_path = 0;
	non_controlling = false;
	traveled = false;
	solved = false;
	level = 0;
	time = 0;
	delay_time = 1;
	prev = 0;
}

size_t Node::find_in(Node *n)
{
	size_t i;
	for(i = 0; i < in.size(); i++) {
		if(in[i] == n)
			return i;
	}
}

Node* Node::find_in_neighbor(Node *n)
{
	if(in.size() == 0)
		return 0;
	if(in.size() == 1)
		return n;
	if(in[0] == n)
		return in[1];
	else return in[0];
}

bool Node::determine_path(Node *n)
{
	switch(this->type)
	{
	case OUTPUT:
		this->out_value = n->out_value;
		this->time = n->time + delay_time;
		this->true_path = 1;
		this->solved = true;
		return true;
	case WIRE:
		this->out_value = n->out_value;
		this->time = n->time + delay_time;
		this->true_path = 1;
		this->solved = true;
		return true;
	case NOT:
		this->out_value = !n->out_value;
		this->time = n->time + delay_time;
		this->true_path = 1;
		this->solved = true;
		return true;
	case NOR:
		if(n->out_value == true) {
			this->out_value = false;
			this->time = n->time + delay_time;
			this->true_path = 1+find_in(n);
			this->solved = true;
			return true;
		}
		else {
			if(non_controlling) {
				Node *m = find_in_neighbor(n);
				this->out_value = true;
				if(m->time <= n->time) { // n is true path
					this->time = n->time + delay_time;
					this->true_path = 1+find_in(n);
					this->solved = true;
					return true;
				}
				else { // m is true path
					this->time = m->time + delay_time;
					this->true_path = 1+find_in(m);
					this->solved = true;
					return true;
				}
			}
			else {
				non_controlling = true;
				return false;
			}
		}
	case NAND:
		if(n->out_value == false) {
			this->out_value = true;
			this->time = n->time + delay_time;
			this->true_path = 1+find_in(n);
			this->solved = true;
			return true;
		}
		else {
			if(non_controlling) {
				Node *m = find_in_neighbor(n);
				this->out_value = false;
				if(m->time <= n->time) { // n is true path
					this->time = n->time + delay_time;
					this->true_path = 1+find_in(n);
					this->solved = true;
					return true;
				}
				else { // m is true path
					this->time = m->time + delay_time;
					this->true_path = 1+find_in(m);
					this->solved = true;
					return true;
				}
			}
			else {
				non_controlling = true;
				return false;
			}
		}
	default: break;
	}
	return false;
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
		n->time = 0;
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
		node->solved = false;
		node->non_controlling = false;
		node->prev = 0;
		node->true_path = 0;
		node->time = 0;
	}
	for ( it = inputs.begin(); it != inputs.end(); it++ )
	{
		Node *node = (*it).second;
		node->traveled = false;
		node->solved = false;
		node->prev = 0;
		node->true_path = 0;
		node->time = 0;
		node->out_value = 1;
	}
	for ( it = outputs.begin(); it != outputs.end(); it++ )
	{
		Node *node = (*it).second;
		node->traveled = false;
		node->solved = false;
		node->prev = 0;
		node->true_path = 0;
		node->time = 0;
	}
	for ( it = wires.begin(); it != wires.end(); it++ )
	{
		Node *node = (*it).second;
		node->traveled = false;
		node->solved = false;
		node->prev = 0;
		node->true_path = 0;
		node->time = 0;
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
	for(itN = u->out.begin(); itN != u->out.end(); itN++)
	{
		if((*itN)->time-(*itN)->delay_time < u->time) {
			if((*itN)->level == 0)
				(*itN)->level = u->level+u->delay_time;
			else {
				if((*itN)->level >= u->level+u->delay_time)
					(*itN)->level = u->level+u->delay_time;
			}
			(*itN)->time = u->time + (*itN)->delay_time;
			(*itN)->prev = u;
			time = this->DFS_Visit((*itN), time);
		}
	}
	time++;
	u->f = time;
        return time;
}
