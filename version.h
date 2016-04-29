#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "28";
	static const char MONTH[] = "04";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.04";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 9;
	static const long MINOR  = 6;
	static const long BUILD  = 217;
	static const long REVISION  = 1214;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 3233;
	#define RC_FILEVERSION 9,6,217,1214
	#define RC_FILEVERSION_STRING "9, 6, 217, 1214\0"
	static const char FULLVERSION_STRING [] = "9.6.217.1214";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 11;
	

}
#endif //VERSION_H
