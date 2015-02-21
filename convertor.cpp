// assuming everything in lower case hex

#include <string>
#include "convertor.h"

int hex_to_int ( string hex ) {
	int a[2];
	int ans;
	ans = 0;
	hex = tolower(hex);
	for ( i = 0; i < 2; i++ ) {
	    if ( hex.at(i) >= '0' && hex.at(i) <= '9' ) {
			a[i] = hex.at(i) - '0';
		} else if ( hex.at(i) >= 'a' && hex.at(i) <= 'f' ) {
			a[i] = hex.at(i) - 'a' + 10;
		}
	}
	ans = a[0]*16 + a[1];
	return ans;
}

string int_to_hex ( int num ) {
	int quo, rem;
	quo = num/16;
	rem = num%16;
	string hex = "00";
	for ( int i = 0; i < 2; i++ ) {
		if ( quo < 10 ) {
			hex.at(i) = quo + '0';
		} else {
			hex.at(i) = quo - 10 + 'a';
		}
	}
	return hex;
}
