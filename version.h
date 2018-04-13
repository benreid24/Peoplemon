#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "12";
	static const char MONTH[] = "04";
	static const char YEAR[] = "2018";
	static const char UBUNTU_VERSION_STYLE[] =  "18.04";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 12;
	static const long MINOR  = 3;
	static const long BUILD  = 461;
	static const long REVISION  = 2527;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 4661;
	#define RC_FILEVERSION 12,3,461,2527
	#define RC_FILEVERSION_STRING "12, 3, 461, 2527\0"
	static const char FULLVERSION_STRING [] = "12.3.461.2527";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 9;
	

}
#endif //VERSION_H
