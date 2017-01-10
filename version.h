#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "10";
	static const char MONTH[] = "01";
	static const char YEAR[] = "2017";
	static const char UBUNTU_VERSION_STYLE[] =  "17.01";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 11;
	static const long MINOR  = 10;
	static const long BUILD  = 326;
	static const long REVISION  = 1732;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 4334;
	#define RC_FILEVERSION 11,10,326,1732
	#define RC_FILEVERSION_STRING "11, 10, 326, 1732\0"
	static const char FULLVERSION_STRING [] = "11.10.326.1732";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 14;
	

}
#endif //VERSION_H
