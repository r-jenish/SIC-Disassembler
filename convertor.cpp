// assuming everything in lower case hex
#include <string>
#include "convertor.h"

std::string lower_to_upper ( std::string hex ) {
	if ( hex.length() == 1 ) {
		char x = hex.at(0);
		hex = "0";
		hex += x;
	} 
	for ( int i = 0; i < 2; i++ ) {
		if ( hex.at(i) >= 'a' && hex.at(i) <= 'f' ) {
			hex.at(i) = hex.at(i) - 'a' + 'A';
		}
	}
	return hex;
}

int hex_to_int ( std::string hex ) {
	hex = lower_to_upper(hex);
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
	int a[2];
	a[0] = num/16;
	a[1] = num%16;
	std::string hex = "00";
	for ( int i = 0; i < 2; i++ ) {
		if ( a[i] < 10 ) {
			hex.at(i) = a[i] + '0';
		} else {
			hex.at(i) = a[i] - 10 + 'A';
		}
	}
	return hex;
}

int hex_to_int6 ( std::string hex ) {
	hex = lower_to_upper(hex);
	std::string temp = "00";
	int a[3];
	int ans;
	for ( int i = 0; i < 3; i++ ) {
		temp.at(0) = hex.at(2*i);
		temp.at(1) = hex.at((2*i)+1);
		a[i] = hex_to_int(temp);
	}
	ans = a[0]*16*16*16*16 + a[1]*16*16 + a[2];
	return ans;
}
