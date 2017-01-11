#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "11";
	static const char MONTH[] = "01";
	static const char YEAR[] = "2017";
	static const char UBUNTU_VERSION_STYLE[] =  "17.01";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 12;
	static const long MINOR  = 0;
	static const long BUILD  = 363;
	static const long REVISION  = 1957;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 4419;
	#define RC_FILEVERSION 12,0,363,1957
	#define RC_FILEVERSION_STRING "12, 0, 363, 1957\0"
	static const char FULLVERSION_STRING [] = "12.0.363.1957";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 16;
	

}
#endif //VERSION_H
