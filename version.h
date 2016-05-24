#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "24";
	static const char MONTH[] = "05";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.05";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 9;
	static const long MINOR  = 9;
	static const long BUILD  = 333;
	static const long REVISION  = 1796;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 3479;
	#define RC_FILEVERSION 9,9,333,1796
	#define RC_FILEVERSION_STRING "9, 9, 333, 1796\0"
	static const char FULLVERSION_STRING [] = "9.9.333.1796";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 22;
	

}
#endif //VERSION_H
