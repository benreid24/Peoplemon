#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "17";
	static const char MONTH[] = "04";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.04";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 9;
	static const long MINOR  = 3;
	static const long BUILD  = 125;
	static const long REVISION  = 708;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 2995;
	#define RC_FILEVERSION 9,3,125,708
	#define RC_FILEVERSION_STRING "9, 3, 125, 708\0"
	static const char FULLVERSION_STRING [] = "9.3.125.708";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 24;
	

}
#endif //VERSION_H
