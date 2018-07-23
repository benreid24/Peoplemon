#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "22";
	static const char MONTH[] = "07";
	static const char YEAR[] = "2018";
	static const char UBUNTU_VERSION_STYLE[] =  "18.07";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 12;
	static const long MINOR  = 7;
	static const long BUILD  = 617;
	static const long REVISION  = 3418;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 5063;
	#define RC_FILEVERSION 12,7,617,3418
	#define RC_FILEVERSION_STRING "12, 7, 617, 3418\0"
	static const char FULLVERSION_STRING [] = "12.7.617.3418";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 25;
	

}
#endif //VERSION_H
