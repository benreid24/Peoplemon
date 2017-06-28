#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "27";
	static const char MONTH[] = "06";
	static const char YEAR[] = "2017";
	static const char UBUNTU_VERSION_STYLE[] =  "17.06";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 12;
	static const long MINOR  = 0;
	static const long BUILD  = 367;
	static const long REVISION  = 1988;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 4451;
	#define RC_FILEVERSION 12,0,367,1988
	#define RC_FILEVERSION_STRING "12, 0, 367, 1988\0"
	static const char FULLVERSION_STRING [] = "12.0.367.1988";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 20;
	

}
#endif //VERSION_H
