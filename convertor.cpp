// assuming everything in lower case hex
#include <string>
#include "convertor.h"

int hex_to_int ( std::string hex ) {
	int a[2];
	int ans;
	ans = 0;
	for ( int i = 0; i < 2; i++ ) {
	    if ( hex.at(i) >= '0' && hex.at(i) <= '9' ) {
			a[i] = hex.at(i) - '0';
		} else if ( hex.at(i) >= 'A' && hex.at(i) <= 'F' ) {
			a[i] = hex.at(i) - 'A' + 10;
		}
	}
	ans = a[0]*16 + a[1];
	return ans;
}

std::string int_to_hex ( int num ) {
	int quo, rem;
	quo = num/16;
	rem = num%16;
	std::string hex = "00";
	for ( int i = 0; i < 2; i++ ) {
		if ( quo < 10 ) {
			hex.at(i) = quo + '0';
		} else {
			hex.at(i) = quo - 10 + 'A';
		}
	}
	return hex;
}
