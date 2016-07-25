#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "24";
	static const char MONTH[] = "07";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.07";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 11;
	static const long MINOR  = 9;
	static const long BUILD  = 294;
	static const long REVISION  = 1534;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 4264;
	#define RC_FILEVERSION 11,9,294,1534
	#define RC_FILEVERSION_STRING "11, 9, 294, 1534\0"
	static const char FULLVERSION_STRING [] = "11.9.294.1534";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 17;
	

}
#endif //VERSION_H
