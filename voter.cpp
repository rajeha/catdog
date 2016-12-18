/***************************
 *  Author: Ahmad Rajeh
 *
 *  Last Updated: 21-11-2016
 ****************************/
#include <iostream>
#include "voter.h"

int isConflict(const Voter& a, const Voter&b ) {	// two voters conflict if one of them hates the other's loved pet
	if (a.love==b.hate || a.hate==b.love)
		return true;
	else 
		return false;
}

ostream& operator << (ostream& s, Voter& v) {	
	return s << "+" << v.love << ",-" << v.hate;
} 
