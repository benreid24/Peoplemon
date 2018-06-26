#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "26";
	static const char MONTH[] = "06";
	static const char YEAR[] = "2018";
	static const char UBUNTU_VERSION_STYLE[] =  "18.06";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 12;
	static const long MINOR  = 5;
	static const long BUILD  = 522;
	static const long REVISION  = 2867;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 4801;
	#define RC_FILEVERSION 12,5,522,2867
	#define RC_FILEVERSION_STRING "12, 5, 522, 2867\0"
	static const char FULLVERSION_STRING [] = "12.5.522.2867";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 0;
	

}
#endif //VERSION_H
