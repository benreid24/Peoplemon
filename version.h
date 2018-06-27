#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "27";
	static const char MONTH[] = "06";
	static const char YEAR[] = "2018";
	static const char UBUNTU_VERSION_STYLE[] =  "18.06";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 12;
	static const long MINOR  = 5;
	static const long BUILD  = 541;
	static const long REVISION  = 2961;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 4869;
	#define RC_FILEVERSION 12,5,541,2961
	#define RC_FILEVERSION_STRING "12, 5, 541, 2961\0"
	static const char FULLVERSION_STRING [] = "12.5.541.2961";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 19;
	

}
#endif //VERSION_H
