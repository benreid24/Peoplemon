#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "25";
	static const char MONTH[] = "06";
	static const char YEAR[] = "2018";
	static const char UBUNTU_VERSION_STYLE[] =  "18.06";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 12;
	static const long MINOR  = 4;
	static const long BUILD  = 519;
	static const long REVISION  = 2847;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 4787;
	#define RC_FILEVERSION 12,4,519,2847
	#define RC_FILEVERSION_STRING "12, 4, 519, 2847\0"
	static const char FULLVERSION_STRING [] = "12.4.519.2847";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 32;
	

}
#endif //VERSION_H
