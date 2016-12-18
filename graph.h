/***************************
 *  Author: Ahmad Rajeh
 *
 *  Last Updated: 21-11-2016
 ****************************/
#ifndef GRAPH_H
#define GRAPH_H

using namespace std;

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

#endif

