#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "04";
	static const char MONTH[] = "07";
	static const char YEAR[] = "2018";
	static const char UBUNTU_VERSION_STYLE[] =  "18.07";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 12;
	static const long MINOR  = 5;
	static const long BUILD  = 547;
	static const long REVISION  = 3018;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 4853;
	#define RC_FILEVERSION 12,5,547,3018
	#define RC_FILEVERSION_STRING "12, 5, 547, 3018\0"
	static const char FULLVERSION_STRING [] = "12.5.547.3018";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 25;
	

}
#endif //VERSION_H
