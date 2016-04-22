#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "21";
	static const char MONTH[] = "04";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.04";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 9;
	static const long MINOR  = 4;
	static const long BUILD  = 167;
	static const long REVISION  = 933;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 3130;
	#define RC_FILEVERSION 9,4,167,933
	#define RC_FILEVERSION_STRING "9, 4, 167, 933\0"
	static const char FULLVERSION_STRING [] = "9.4.167.933";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 31;
	

}
#endif //VERSION_H
