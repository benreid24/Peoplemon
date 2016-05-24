#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "24";
	static const char MONTH[] = "05";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.05";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 10;
	static const long MINOR  = 0;
	static const long BUILD  = 7;
	static const long REVISION  = 40;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 3495;
	#define RC_FILEVERSION 10,0,7,40
	#define RC_FILEVERSION_STRING "10, 0, 7, 40\0"
	static const char FULLVERSION_STRING [] = "10.0.7.40";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 30;
	

}
#endif //VERSION_H
