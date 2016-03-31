#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "31";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.03";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 9;
	static const long MINOR  = 2;
	static const long BUILD  = 78;
	static const long REVISION  = 426;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 2903;
	#define RC_FILEVERSION 9,2,78,426
	#define RC_FILEVERSION_STRING "9, 2, 78, 426\0"
	static const char FULLVERSION_STRING [] = "9.2.78.426";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 12;
	

}
#endif //VERSION_H
