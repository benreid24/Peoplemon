#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "30";
	static const char MONTH[] = "06";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.06";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 11;
	static const long MINOR  = 2;
	static const long BUILD  = 33;
	static const long REVISION  = 198;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 3750;
	#define RC_FILEVERSION 11,2,33,198
	#define RC_FILEVERSION_STRING "11, 2, 33, 198\0"
	static const char FULLVERSION_STRING [] = "11.2.33.198";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 1;
	

}
#endif //VERSION_H
