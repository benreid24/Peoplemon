#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "28";
	static const char MONTH[] = "06";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.06";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 11;
	static const long MINOR  = 1;
	static const long BUILD  = 1;
	static const long REVISION  = 1;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 3708;
	#define RC_FILEVERSION 11,1,1,1
	#define RC_FILEVERSION_STRING "11, 1, 1, 1\0"
	static const char FULLVERSION_STRING [] = "11.1.1.1";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 4;
	

}
#endif //VERSION_H
