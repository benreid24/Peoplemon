#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "08";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2018";
	static const char UBUNTU_VERSION_STYLE[] =  "18.02";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 12;
	static const long MINOR  = 2;
	static const long BUILD  = 423;
	static const long REVISION  = 2320;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 4624;
	#define RC_FILEVERSION 12,2,423,2320
	#define RC_FILEVERSION_STRING "12, 2, 423, 2320\0"
	static const char FULLVERSION_STRING [] = "12.2.423.2320";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 6;
	

}
#endif //VERSION_H
