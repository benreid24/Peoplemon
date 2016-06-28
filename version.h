#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "28";
	static const char MONTH[] = "06";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.06";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 10;
	static const long MINOR  = 5;
	static const long BUILD  = 111;
	static const long REVISION  = 619;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 3687;
	#define RC_FILEVERSION 10,5,111,619
	#define RC_FILEVERSION_STRING "10, 5, 111, 619\0"
	static const char FULLVERSION_STRING [] = "10.5.111.619";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 29;
	

}
#endif //VERSION_H
