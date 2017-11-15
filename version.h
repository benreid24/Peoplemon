#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "14";
	static const char MONTH[] = "11";
	static const char YEAR[] = "2017";
	static const char UBUNTU_VERSION_STYLE[] =  "17.11";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 12;
	static const long MINOR  = 1;
	static const long BUILD  = 388;
	static const long REVISION  = 2126;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 4501;
	#define RC_FILEVERSION 12,1,388,2126
	#define RC_FILEVERSION_STRING "12, 1, 388, 2126\0"
	static const char FULLVERSION_STRING [] = "12.1.388.2126";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 6;
	

}
#endif //VERSION_H
