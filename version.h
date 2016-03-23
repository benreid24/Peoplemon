#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "22";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.03";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 8;
	static const long MINOR  = 0;
	static const long BUILD  = 321;
	static const long REVISION  = 1748;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 2724;
	#define RC_FILEVERSION 8,0,321,1748
	#define RC_FILEVERSION_STRING "8, 0, 321, 1748\0"
	static const char FULLVERSION_STRING [] = "8.0.321.1748";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 1;
	

}
#endif //VERSION_H
