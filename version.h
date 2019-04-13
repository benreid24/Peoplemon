#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "12";
	static const char MONTH[] = "04";
	static const char YEAR[] = "2019";
	static const char UBUNTU_VERSION_STYLE[] =  "19.04";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 12;
	static const long MINOR  = 9;
	static const long BUILD  = 694;
	static const long REVISION  = 3837;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 5164;
	#define RC_FILEVERSION 12,9,694,3837
	#define RC_FILEVERSION_STRING "12, 9, 694, 3837\0"
	static const char FULLVERSION_STRING [] = "12.9.694.3837";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 32;
	

}
#endif //VERSION_H
