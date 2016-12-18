/***************************
 *  Author: Ahmad Rajeh
 *
 *  Last Updated: 21-11-2016
 ****************************/
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <set>

#define BLACK 1
#define WHITE 0
#define HAPPY 1
#define SAD 0
#define INF 99

using namespace std;

///////////////
//	edge.h	//
/////////////
class Edge {	// an edge tells you where you're connecting and with what weight
	private:
		int vert, wt;

	public:
		Edge() { vert = -1; wt = -1; }
		Edge(const int v, const int w) { vert = v; wt = w; }
		int vertex() const { return vert; }
		int weight() const { return wt; }
		void setWt (const int w) { wt = w; }
		void modWt (const int w) { wt += w; }
};

///////////////
//	voter.h	//
//////////////
struct Voter {	// a voter has a loved pet, a hated pet, and is either happy or sad 
	string love;
	string hate;
	int happy;

	Voter(const string& l, const string& h) : love(l), hate(h), happy(HAPPY) {}	// all voters start happy 
};

int isConflict(const Voter& a, const Voter& b);
ostream& operator << (ostream& s, Voter& v);

/////////////////
//	voter.cpp //
///////////////
int isConflict(const Voter& a, const Voter&b ) {	// two voters conflict if one of them hates the other's loved pet
	if (a.love==b.hate || a.hate==b.love)
		return true;
	else 
		return false;
}

ostream& operator << (ostream& s, Voter& v) {
	return s << "+" << v.love << ",-" << v.hate;
} 

///////////////
//	graph.h	//
/////////////
class  Graph {	// adjacency list graph
	private:
		int V;
		vector<Edge*>* vertices;	
		int* color;		// colors of edges (white=unvisited,black=visited)
		int* parent;	// parents for maxflow

	public:
		Graph(const int v);
		Graph(istream& in, vector<Voter>& cats, vector<Voter>& dogs);	// constructs graph from fromated input
		~Graph();

		void initialize(const int v);
		int v() const;
		void printGraph() const;	// for debugging 
		void addEdge(const int i, const int j, const int w);
		void modEdge(const int i, const int j, const int w);	// modifies an edge
		int hasEdge(const int i) const;	// for debugging 		
		Edge* getEdge(const int i, const int j) const;
		int getColor(const int i) const;

		int bfs(int s, const int t);
		int maxFlow(const int s, const int t);

		void printSoln(ostream& out, const vector<Voter>& cats, const vector<Voter>& dogs);
		void printTrace(ostream& out, const vector<Voter>& cats, const vector<Voter>& dogs);	
};

/////////////////
//	graph.cpp //
///////////////
Graph::Graph(const int v) {
	initialize(v);
}

Graph::Graph(istream& in, vector<Voter>& cats, vector<Voter>& dogs) {
	// empty cats and dogs (for when it's not the first test case)	
	cats.clear();
	dogs.clear();

	string line;
	int voters;
	getline(in,line);
	istringstream iss(line);	
	iss >> voters >> voters >> voters;	// get num of voters

	initialize(voters + 2);	// +2 because of source and sink vertices

	string love, hate;
	for (int i=0; i<voters; i++) {
		getline(in, line);
		istringstream iss(line);
		iss >> love;
		iss >> hate;
		Voter v = Voter(love, hate);	
		if (love[0]=='C')
			cats.push_back(v);
		else
			dogs.push_back(v);
	}

	// source and sink take the very last couple of indecies in the graph
	int s = cats.size() + dogs.size();
	int t = s + 1;

	for (int i=0; i<cats.size(); i++) {
		addEdge(s, i, 1);	// connect all catlovers to source
		for (int j=0; j<dogs.size(); j++) {
			addEdge(j + cats.size(), t, 1);	// connect all doglovers to sink
			if (isConflict(cats[i],dogs[j])) 
				addEdge(i, j + cats.size(), INF);	// connect conflicting voters
		}
	}
}

void Graph::initialize(const int v) {
	V = v; 
	vertices = new vector<Edge*>[v];
	parent = new int[v]; 
	color = new int[v];	
}

Graph::~Graph() {
	delete [] color;
	delete [] parent;
	for (int i=0; i<V; i++) vertices[i].clear();
	delete [] vertices;
}

int Graph::v() const {
	return V;
}

void Graph::printGraph() const {	// for debugging 
	int i, j;
	Edge* e;
	for (i=0; i<V; i++) {
		for (j=0; j<V; j++) {
			if ((e=getEdge(i,j))!=NULL) cerr << e->weight() << " ";
			else cerr << "0" << " ";
			cerr << endl;
		}
	}
}

void Graph::addEdge(const int i, const int j, const int w) {
	for (Edge* it : vertices[i])
		if ((*it).vertex() == j) {
			(*it).setWt(w);
			return;
		}

	vertices[i].push_back(new Edge(j,w));
}

void Graph::modEdge(const int i, const int j, const int w) {	// modifies an edges weight
	for (Edge* it : vertices[i])
		if ((*it).vertex() == j) {
			(*it).modWt(w);
			return;
		}
}

Edge* Graph::getEdge(const int i, const int j) const {
	for (Edge* it : vertices[i])
		if ((*it).vertex() == j)
			return &(*it);

	return NULL;	
}

int Graph::hasEdge(const int i) const {	// for debugging 
	if (!vertices[i].empty()) return 1;
	for (int j=0; j<V; j++) 
		for (Edge* it : vertices[j])
			if ((*it).vertex() == i)
				return 1;

	return 0;
}

int Graph::getColor(const int i) const {
	return color[i];
}

int Graph::bfs(int s, const int t) {
	int i;
	queue<int> q;
	for (int i=0; i<V; i++) color[i] = WHITE;	// all vertices start white/unvisited

	color[s] = BLACK;
	parent[s] = -1;	
	q.push(s);

	while (!q.empty()) {
		s = q.front();
		q.pop();
		for(Edge* it : vertices[s]) {
			int j = (*it).vertex();
			if(!color[j] && (*it).weight()>0) {
				color[j] = BLACK;
				parent[j] = s;
				q.push(j);
			}
		}	
	}

	return (color[t] == BLACK);	
}

int Graph::maxFlow(const int s, const int t) {
	int u, v, pathFlow;
	int flow = 0;	// max flow starts at 0
	Graph* residual = this;

	while (residual->bfs(s, t)) {	// while there exists an augmenting path
		pathFlow = INF;
		v = t;
		while (v!=s) {	// find bottleneck weight along the path
			u = parent[v];
			pathFlow = min(pathFlow, residual->getEdge(u,v)->weight());
			v = u;
		}

		v = t;	
		while (v!=s) {	// update edge weights in the path + add back-edges
			u = parent[v];
			residual->modEdge(u,v,-pathFlow);
			residual->addEdge(v,u,pathFlow);
			v = u;
		}

		flow += pathFlow;	// add the augmenting to max flow
	}

	return flow;
}

void Graph::printSoln(ostream& out, const vector<Voter>& cats, const vector<Voter>& dogs) {
	// soln = total voters - max flow
	out << cats.size() + dogs.size() - maxFlow(V-2, V-1) << endl;
}

void Graph::printTrace(ostream& out, const vector<Voter>& cats, const vector<Voter>& dogs) {
	// join catlovers and doglovers into one vector
	vector<Voter> all = cats;
	all.insert(all.end(), dogs.begin(), dogs.end());

	// catlovers not reachable from s and doglovers reachable
	// from s belong in the VC, so they will be sad 
	for (int i=0; i<cats.size(); i++) 
		if (!getColor(i))
			all[i].happy = SAD;
	for (int i=0; i<dogs.size(); i++)
		if (getColor(i+cats.size()))
			all[i+cats.size()].happy = SAD;


	// find the set of pets to keep from happy voters
	set<string> keep;
	for (int i=0; i<all.size(); i++)
		if (all[i].happy) 
			keep.insert(all[i].love);

	// print out trace info into stderr
	for (string it : keep)
		out << "Keeping:\t" << it << endl;
	for (int i=0; i<all.size(); i++)
		if (all[i].happy)
			out << "Happy person:\t" << all[i] << endl;
	out << endl;
}

/////////////////
//	main.cpp  //
///////////////
int main() {
	string line;
	vector<Voter> cats, dogs, all;

	getline(cin, line);
	int cases = atoi(&line[0]);	// get num of test cases
	for (int i=0; i<cases; i++) {
		Graph G = Graph(cin, cats, dogs);

		G.printSoln(cout, cats, dogs);
		G.printTrace(cerr, cats, dogs);
	} 		
}
