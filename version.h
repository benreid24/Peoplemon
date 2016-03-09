#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "08";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.03";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 7;
	static const long MINOR  = 7;
	static const long BUILD  = 197;
	static const long REVISION  = 1001;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 2378;
	#define RC_FILEVERSION 7,7,197,1001
	#define RC_FILEVERSION_STRING "7, 7, 197, 1001\0"
	static const char FULLVERSION_STRING [] = "7.7.197.1001";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 17;
	

}
#endif //VERSION_H
