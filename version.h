#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "19";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.03";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 7;
	static const long MINOR  = 9;
	static const long BUILD  = 261;
	static const long REVISION  = 1387;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 2583;
	#define RC_FILEVERSION 7,9,261,1387
	#define RC_FILEVERSION_STRING "7, 9, 261, 1387\0"
	static const char FULLVERSION_STRING [] = "7.9.261.1387";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 11;
	

}
#endif //VERSION_H
