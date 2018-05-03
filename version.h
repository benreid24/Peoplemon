#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "03";
	static const char MONTH[] = "05";
	static const char YEAR[] = "2018";
	static const char UBUNTU_VERSION_STYLE[] =  "18.05";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 12;
	static const long MINOR  = 4;
	static const long BUILD  = 488;
	static const long REVISION  = 2665;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 4723;
	#define RC_FILEVERSION 12,4,488,2665
	#define RC_FILEVERSION_STRING "12, 4, 488, 2665\0"
	static const char FULLVERSION_STRING [] = "12.4.488.2665";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 1;
	

}
#endif //VERSION_H
