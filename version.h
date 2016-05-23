#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "23";
	static const char MONTH[] = "05";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.05";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 9;
	static const long MINOR  = 9;
	static const long BUILD  = 328;
	static const long REVISION  = 1760;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 3465;
	#define RC_FILEVERSION 9,9,328,1760
	#define RC_FILEVERSION_STRING "9, 9, 328, 1760\0"
	static const char FULLVERSION_STRING [] = "9.9.328.1760";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 17;
	

}
#endif //VERSION_H
