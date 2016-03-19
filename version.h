#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "19";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.03";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 7;
	static const long MINOR  = 9;
	static const long BUILD  = 263;
	static const long REVISION  = 1406;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 2587;
	#define RC_FILEVERSION 7,9,263,1406
	#define RC_FILEVERSION_STRING "7, 9, 263, 1406\0"
	static const char FULLVERSION_STRING [] = "7.9.263.1406";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 13;
	

}
#endif //VERSION_H
