#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "15";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2018";
	static const char UBUNTU_VERSION_STYLE[] =  "18.02";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 12;
	static const long MINOR  = 2;
	static const long BUILD  = 435;
	static const long REVISION  = 2408;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 4644;
	#define RC_FILEVERSION 12,2,435,2408
	#define RC_FILEVERSION_STRING "12, 2, 435, 2408\0"
	static const char FULLVERSION_STRING [] = "12.2.435.2408";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 18;
	

}
#endif //VERSION_H
