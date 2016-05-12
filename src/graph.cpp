#include "graph.h"


Edge::Edge(Node *a, Node *b, const int& w)
{
	if ( a->id <= b->id ) { node[0] = a; node[1] = b; }
	else { node[0] = b; node[1] = a; }
	weight = w;
}

bool Edge::operator < (const Edge& rhs) const{

	int id1a = node[0]->id;
	int id1b = node[1]->id;

	int id2a = rhs.node[0]->id;
	int id2b = rhs.node[1]->id;


	if ( id1a == id2a ) return id1b < id2b;
	if ( id1a == id2b ) return id1b < id2a;
	if ( id1b == id2a ) return id1a < id2b;
	if ( id1b == id2b ) return id1a < id2a;

	return weight < rhs.weight;
}
Node * Edge::getNeighbor(Node *n)
{
	if ( node[0] == n ) return node[1];
	if ( node[1] == n ) return node[0];

	return 0;	
}


Node::Node(const int& i)
{
	id = i;
	traveled = false;
	d = DIS_INF;
	prev = 0;
}

void Node::addEdge(Edge *e)
{
	edge.push_back(e);
}

bool edgeComp( const Edge* A, const Edge* B ){
	if ( (*A) < (*B) ) return true;
	return false;
}


void Node::sortEdge()
{
    sort(edge.begin(), edge.end(), edgeComp);
}

bool nodeComp (const Node* lhs, const Node* rhs) {
	return lhs->d < rhs->d;
}

Graph::Graph(const string& n)
{
	name = n;
}

Graph::~Graph()
{
	vector<Edge *>::iterator itE;
	for ( itE = edges.begin() ; itE != edges.end() ; itE++ )
	{
		delete (*itE);
		(*itE) = 0;
	}
	
	map<int, Node *>::iterator itN;
	for ( itN = nodes.begin() ; itN != nodes.end() ; itN++ )
	{
		delete (*itN).second;
		(*itN).second = 0;
	}
}
		
void Graph::addEdge(const int& v1, const int& v2, const int& w)
{
	Node *a, *b;
	map<int, Node *>::iterator it = nodes.find(v1);
	if ( it != nodes.end() )
		a = (*it).second;
	else
	{
		a = new Node(v1);
		nodes[v1] = a;
	}

	it = nodes.find(v2);
	if ( it != nodes.end() )
		b = (*it).second;
	else
	{
		b = new Node(v2);
		nodes[v2] = b;
	}

	Edge *e = new Edge(a, b, w);
	edges.push_back(e);

	a->edge.push_back(e);
	b->edge.push_back(e);

}
	
void Graph::sortEdgesOfNode()
{
	map<int, Node *>::iterator it;
	for ( it = nodes.begin() ; it != nodes.end() ; it++ )
	{
		Node *node = (*it).second;
		node->sortEdge();
	}

}

bool edgeCompByW( const Edge* A, const Edge* B ){
	if ( A->weight < B->weight ) return true;
	return false;
}

void Graph::sortEdgesByWeight()
{
    sort(edges.begin(), edges.end(), edgeCompByW);
}
void Graph::init()
{
	map<int, Node *>::iterator itN;
	for ( itN = nodes.begin() ; itN != nodes.end() ; itN++ )
	{
		Node *node = (*itN).second;
		node->traveled = false;
		node->d = DIS_INF;
		node->i = 0;
		node->f = 0;
		node->prev = 0;
	}
	
}

Node * Graph::getNodeById(const int& id)
{
	return nodes[id];
}

int Graph::DFS_Visit (Node* u, int time, ofstream& o)
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
}
