#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "02";
	static const char MONTH[] = "05";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.05";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 9;
	static const long MINOR  = 7;
	static const long BUILD  = 258;
	static const long REVISION  = 1400;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 3326;
	#define RC_FILEVERSION 9,7,258,1400
	#define RC_FILEVERSION_STRING "9, 7, 258, 1400\0"
	static const char FULLVERSION_STRING [] = "9.7.258.1400";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 17;
	

}
#endif //VERSION_H
