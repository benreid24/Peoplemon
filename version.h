#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "04";
	static const char MONTH[] = "07";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.07";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 11;
	static const long MINOR  = 5;
	static const long BUILD  = 168;
	static const long REVISION  = 888;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 3988;
	#define RC_FILEVERSION 11,5,168,888
	#define RC_FILEVERSION_STRING "11, 5, 168, 888\0"
	static const char FULLVERSION_STRING [] = "11.5.168.888";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 31;
	

}
#endif //VERSION_H
