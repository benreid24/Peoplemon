#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "03";
	static const char MONTH[] = "07";
	static const char YEAR[] = "2018";
	static const char UBUNTU_VERSION_STYLE[] =  "18.07";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 12;
	static const long MINOR  = 5;
	static const long BUILD  = 537;
	static const long REVISION  = 2945;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 4830;
	#define RC_FILEVERSION 12,5,537,2945
	#define RC_FILEVERSION_STRING "12, 5, 537, 2945\0"
	static const char FULLVERSION_STRING [] = "12.5.537.2945";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 15;
	

}
#endif //VERSION_H
