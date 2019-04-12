#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "11";
	static const char MONTH[] = "04";
	static const char YEAR[] = "2019";
	static const char UBUNTU_VERSION_STYLE[] =  "19.04";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 12;
	static const long MINOR  = 9;
	static const long BUILD  = 686;
	static const long REVISION  = 3786;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 5157;
	#define RC_FILEVERSION 12,9,686,3786
	#define RC_FILEVERSION_STRING "12, 9, 686, 3786\0"
	static const char FULLVERSION_STRING [] = "12.9.686.3786";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 24;
	

}
#endif //VERSION_H
