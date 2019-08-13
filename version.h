#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "13";
	static const char MONTH[] = "08";
	static const char YEAR[] = "2019";
	static const char UBUNTU_VERSION_STYLE[] =  "19.08";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 14;
	static const long MINOR  = 5;
	static const long BUILD  = 122;
	static const long REVISION  = 652;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 5481;
	#define RC_FILEVERSION 14,5,122,652
	#define RC_FILEVERSION_STRING "14, 5, 122, 652\0"
	static const char FULLVERSION_STRING [] = "14.5.122.652";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 10;
	

}
#endif //VERSION_H
