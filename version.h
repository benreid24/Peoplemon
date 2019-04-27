#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "27";
	static const char MONTH[] = "04";
	static const char YEAR[] = "2019";
	static const char UBUNTU_VERSION_STYLE[] =  "19.04";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 14;
	static const long MINOR  = 2;
	static const long BUILD  = 21;
	static const long REVISION  = 115;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 5315;
	#define RC_FILEVERSION 14,2,21,115
	#define RC_FILEVERSION_STRING "14, 2, 21, 115\0"
	static const char FULLVERSION_STRING [] = "14.2.21.115";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 14;
	

}
#endif //VERSION_H
