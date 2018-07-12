#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "11";
	static const char MONTH[] = "07";
	static const char YEAR[] = "2018";
	static const char UBUNTU_VERSION_STYLE[] =  "18.07";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 12;
	static const long MINOR  = 6;
	static const long BUILD  = 562;
	static const long REVISION  = 3102;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 4908;
	#define RC_FILEVERSION 12,6,562,3102
	#define RC_FILEVERSION_STRING "12, 6, 562, 3102\0"
	static const char FULLVERSION_STRING [] = "12.6.562.3102";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 5;
	

}
#endif //VERSION_H
