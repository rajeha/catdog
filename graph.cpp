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
#include "edge.h"
#include "voter.h"
#include "graph.h"

#define WHITE 0
#define BLACK 1
#define INF 99

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
