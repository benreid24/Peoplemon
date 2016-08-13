#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "13";
	static const char MONTH[] = "08";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.08";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 11;
	static const long MINOR  = 9;
	static const long BUILD  = 309;
	static const long REVISION  = 1628;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 4283;
	#define RC_FILEVERSION 11,9,309,1628
	#define RC_FILEVERSION_STRING "11, 9, 309, 1628\0"
	static const char FULLVERSION_STRING [] = "11.9.309.1628";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 32;
	

}
#endif //VERSION_H
