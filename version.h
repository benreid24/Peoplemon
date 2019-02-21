#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "20";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2019";
	static const char UBUNTU_VERSION_STYLE[] =  "19.02";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 12;
	static const long MINOR  = 8;
	static const long BUILD  = 638;
	static const long REVISION  = 3520;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 5099;
	#define RC_FILEVERSION 12,8,638,3520
	#define RC_FILEVERSION_STRING "12, 8, 638, 3520\0"
	static const char FULLVERSION_STRING [] = "12.8.638.3520";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 11;
	

}
#endif //VERSION_H
