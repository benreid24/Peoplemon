#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "29";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.03";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 9;
	static const long MINOR  = 1;
	static const long BUILD  = 59;
	static const long REVISION  = 331;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 2862;
	#define RC_FILEVERSION 9,1,59,331
	#define RC_FILEVERSION_STRING "9, 1, 59, 331\0"
	static const char FULLVERSION_STRING [] = "9.1.59.331";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 28;
	

}
#endif //VERSION_H
