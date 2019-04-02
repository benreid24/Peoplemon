#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "01";
	static const char MONTH[] = "04";
	static const char YEAR[] = "2019";
	static const char UBUNTU_VERSION_STYLE[] =  "19.04";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 12;
	static const long MINOR  = 8;
	static const long BUILD  = 645;
	static const long REVISION  = 3561;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 5107;
	#define RC_FILEVERSION 12,8,645,3561
	#define RC_FILEVERSION_STRING "12, 8, 645, 3561\0"
	static const char FULLVERSION_STRING [] = "12.8.645.3561";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 18;
	

}
#endif //VERSION_H
