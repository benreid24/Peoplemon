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
	static const long BUILD  = 16;
	static const long REVISION  = 87;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 5310;
	#define RC_FILEVERSION 14,2,16,87
	#define RC_FILEVERSION_STRING "14, 2, 16, 87\0"
	static const char FULLVERSION_STRING [] = "14.2.16.87";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 9;
	

}
#endif //VERSION_H
