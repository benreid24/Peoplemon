#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "18";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.03";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 7;
	static const long MINOR  = 8;
	static const long BUILD  = 248;
	static const long REVISION  = 1316;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 2566;
	#define RC_FILEVERSION 7,8,248,1316
	#define RC_FILEVERSION_STRING "7, 8, 248, 1316\0"
	static const char FULLVERSION_STRING [] = "7.8.248.1316";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 33;
	

}
#endif //VERSION_H
