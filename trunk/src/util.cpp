#include "util.hpp"

using namespace std;
namespace CDAO {

string labelMaker(  string tag_type ){
	static int serial_number = 0;

	return tag_type + number_to_string( ++serial_number );
}

string number_to_string( int number ){
   
	const int NUM_SIZE = 13;
	const int NUM_ASIZE = NUM_SIZE -1;
 	char str_number[NUM_SIZE];
	snprintf( str_number, NUM_ASIZE, "%d", number );

	return string(str_number);
}
}
