#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "21";
	static const char MONTH[] = "07";
	static const char YEAR[] = "2019";
	static const char UBUNTU_VERSION_STYLE[] =  "19.07";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 14;
	static const long MINOR  = 3;
	static const long BUILD  = 52;
	static const long REVISION  = 286;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 5371;
	#define RC_FILEVERSION 14,3,52,286
	#define RC_FILEVERSION_STRING "14, 3, 52, 286\0"
	static const char FULLVERSION_STRING [] = "14.3.52.286";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 10;
	

}
#endif //VERSION_H
