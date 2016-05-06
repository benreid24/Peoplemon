#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "06";
	static const char MONTH[] = "05";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.05";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 9;
	static const long MINOR  = 8;
	static const long BUILD  = 287;
	static const long REVISION  = 1568;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 3396;
	#define RC_FILEVERSION 9,8,287,1568
	#define RC_FILEVERSION_STRING "9, 8, 287, 1568\0"
	static const char FULLVERSION_STRING [] = "9.8.287.1568";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 11;
	

}
#endif //VERSION_H
