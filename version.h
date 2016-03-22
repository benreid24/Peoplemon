#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "22";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.03";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 7;
	static const long MINOR  = 10;
	static const long BUILD  = 308;
	static const long REVISION  = 1662;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 2701;
	#define RC_FILEVERSION 7,10,308,1662
	#define RC_FILEVERSION_STRING "7, 10, 308, 1662\0"
	static const char FULLVERSION_STRING [] = "7.10.308.1662";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 23;
	

}
#endif //VERSION_H
