#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "17";
	static const char MONTH[] = "08";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.08";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 11;
	static const long MINOR  = 10;
	static const long BUILD  = 323;
	static const long REVISION  = 1716;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 4321;
	#define RC_FILEVERSION 11,10,323,1716
	#define RC_FILEVERSION_STRING "11, 10, 323, 1716\0"
	static const char FULLVERSION_STRING [] = "11.10.323.1716";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 11;
	

}
#endif //VERSION_H
