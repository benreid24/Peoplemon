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
	static const long MINOR  = 10;
	static const long BUILD  = 302;
	static const long REVISION  = 1639;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 2688;
	#define RC_FILEVERSION 7,10,302,1639
	#define RC_FILEVERSION_STRING "7, 10, 302, 1639\0"
	static const char FULLVERSION_STRING [] = "7.10.302.1639";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 17;
	

}
#endif //VERSION_H
