/***************************
 *  Author: Ahmad Rajeh
 *
 *  Last Updated: 21-11-2016
 ****************************/
#ifndef VOTER_H
#define VOTER_H

#define HAPPY 1
#define SAD 0

using namespace std;

struct Voter {	// a voter has a loved pet, a hated pet, and is either happy or sad 
	string love;
	string hate;
	int happy;

	Voter(const string& l, const string& h) : love(l), hate(h), happy(HAPPY) {}	// all voters start happy 
};

int isConflict(const Voter& a, const Voter& b);
ostream& operator << (ostream& s, Voter& v);

#endif
