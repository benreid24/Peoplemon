#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "20";
	static const char MONTH[] = "05";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.05";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 9;
	static const long MINOR  = 8;
	static const long BUILD  = 303;
	static const long REVISION  = 1642;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 3425;
	#define RC_FILEVERSION 9,8,303,1642
	#define RC_FILEVERSION_STRING "9, 8, 303, 1642\0"
	static const char FULLVERSION_STRING [] = "9.8.303.1642";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 27;
	

}
#endif //VERSION_H
