#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "08";
	static const char MONTH[] = "08";
	static const char YEAR[] = "2019";
	static const char UBUNTU_VERSION_STYLE[] =  "19.08";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 14;
	static const long MINOR  = 5;
	static const long BUILD  = 116;
	static const long REVISION  = 625;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 5476;
	#define RC_FILEVERSION 14,5,116,625
	#define RC_FILEVERSION_STRING "14, 5, 116, 625\0"
	static const char FULLVERSION_STRING [] = "14.5.116.625";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 4;
	

}
#endif //VERSION_H
