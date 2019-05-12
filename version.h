#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "12";
	static const char MONTH[] = "05";
	static const char YEAR[] = "2019";
	static const char UBUNTU_VERSION_STYLE[] =  "19.05";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 14;
	static const long MINOR  = 2;
	static const long BUILD  = 23;
	static const long REVISION  = 132;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 5317;
	#define RC_FILEVERSION 14,2,23,132
	#define RC_FILEVERSION_STRING "14, 2, 23, 132\0"
	static const char FULLVERSION_STRING [] = "14.2.23.132";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 16;
	

}
#endif //VERSION_H
