#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "25";
	static const char MONTH[] = "01";
	static const char YEAR[] = "2018";
	static const char UBUNTU_VERSION_STYLE[] =  "18.01";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 12;
	static const long MINOR  = 1;
	static const long BUILD  = 405;
	static const long REVISION  = 2210;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 4583;
	#define RC_FILEVERSION 12,1,405,2210
	#define RC_FILEVERSION_STRING "12, 1, 405, 2210\0"
	static const char FULLVERSION_STRING [] = "12.1.405.2210";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 23;
	

}
#endif //VERSION_H
