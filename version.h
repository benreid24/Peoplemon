#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "19";
	static const char MONTH[] = "04";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.04";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 9;
	static const long MINOR  = 4;
	static const long BUILD  = 145;
	static const long REVISION  = 816;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 3058;
	#define RC_FILEVERSION 9,4,145,816
	#define RC_FILEVERSION_STRING "9, 4, 145, 816\0"
	static const char FULLVERSION_STRING [] = "9.4.145.816";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 9;
	

}
#endif //VERSION_H
