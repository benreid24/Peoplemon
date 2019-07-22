#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "22";
	static const char MONTH[] = "07";
	static const char YEAR[] = "2019";
	static const char UBUNTU_VERSION_STYLE[] =  "19.07";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 14;
	static const long MINOR  = 4;
	static const long BUILD  = 98;
	static const long REVISION  = 511;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 5453;
	#define RC_FILEVERSION 14,4,98,511
	#define RC_FILEVERSION_STRING "14, 4, 98, 511\0"
	static const char FULLVERSION_STRING [] = "14.4.98.511";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 21;
	

}
#endif //VERSION_H
