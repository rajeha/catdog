/***************************
 *  Author: Ahmad Rajeh
 *
 *  Last Updated: 21-11-2016
 ****************************/
#include <iostream>
#include <vector>
#include "edge.h"
#include "voter.h"
#include "graph.h"

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

