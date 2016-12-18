/***************************
 *  Author: Ahmad Rajeh
 *
 *  Last Updated: 21-11-2016
 ****************************/
#ifndef EDGE_H
#define EDGE_H

using namespace std;

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

#endif
