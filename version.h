#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "18";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.03";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 7;
	static const long MINOR  = 8;
	static const long BUILD  = 224;
	static const long REVISION  = 1176;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 2499;
	#define RC_FILEVERSION 7,8,224,1176
	#define RC_FILEVERSION_STRING "7, 8, 224, 1176\0"
	static const char FULLVERSION_STRING [] = "7.8.224.1176";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 9;
	

}
#endif //VERSION_H
