#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "08";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2018";
	static const char UBUNTU_VERSION_STYLE[] =  "18.03";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 12;
	static const long MINOR  = 2;
	static const long BUILD  = 450;
	static const long REVISION  = 2479;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 4643;
	#define RC_FILEVERSION 12,2,450,2479
	#define RC_FILEVERSION_STRING "12, 2, 450, 2479\0"
	static const char FULLVERSION_STRING [] = "12.2.450.2479";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 33;
	

}
#endif //VERSION_H
