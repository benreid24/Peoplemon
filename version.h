#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "21";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.03";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 7;
	static const long MINOR  = 9;
	static const long BUILD  = 277;
	static const long REVISION  = 1486;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 2618;
	#define RC_FILEVERSION 7,9,277,1486
	#define RC_FILEVERSION_STRING "7, 9, 277, 1486\0"
	static const char FULLVERSION_STRING [] = "7.9.277.1486";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 27;
	

}
#endif //VERSION_H
