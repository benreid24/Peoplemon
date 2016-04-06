#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "05";
	static const char MONTH[] = "04";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.04";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 9;
	static const long MINOR  = 2;
	static const long BUILD  = 95;
	static const long REVISION  = 519;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 2941;
	#define RC_FILEVERSION 9,2,95,519
	#define RC_FILEVERSION_STRING "9, 2, 95, 519\0"
	static const char FULLVERSION_STRING [] = "9.2.95.519";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 29;
	

}
#endif //VERSION_H
