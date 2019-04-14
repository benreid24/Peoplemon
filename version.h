#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "14";
	static const char MONTH[] = "04";
	static const char YEAR[] = "2019";
	static const char UBUNTU_VERSION_STYLE[] =  "19.04";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 13;
	static const long MINOR  = 0;
	static const long BUILD  = 746;
	static const long REVISION  = 4156;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 5213;
	#define RC_FILEVERSION 13,0,746,4156
	#define RC_FILEVERSION_STRING "13, 0, 746, 4156\0"
	static const char FULLVERSION_STRING [] = "13.0.746.4156";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 14;
	

}
#endif //VERSION_H
