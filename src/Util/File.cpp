#include "Util/File.hpp"
#include <iostream>

bool FileExists(std::string filename)
{
    std::ifstream file(filename.c_str());
    return file.good();
}

File::File()
{
    //ctor
}

File::File(std::string name, OpenMode mode)
{
    setFile(name, mode);
}

File::~File()
{
    file.close();
}

void File::setFile(std::string name, OpenMode mode)
{
    if (file.is_open())
        file.close();

    if (mode==In)
        file.open(name.c_str(), std::ios::in|std::ios::binary);
    else
    {
        FileExists(name); //ensure file exists
        file.open(name.c_str(), std::ios::binary | std::ios::out);
    }

    if (!file.good())
        std::cout << "Failed to open datafile: " << name << '\n';
}

void File::close()
{
    file.close();
}

std::string File::getExtension(std::string file)
{
    unsigned int i = 0;
    while (i<file.size() && file[i]!='.')
        ++i;
    if (i+1<file.size())
        return file.substr(i+1);
    return "";
}

std::string File::getBaseName(std::string file)
{
    std::string ret;
    for (unsigned int i = 0; i<file.size(); ++i)
    {
        if (file[i]=='.')
            return ret;
        ret.push_back(file[i]);
        if (file[i]=='/' || file[i]=='\\')
            ret.clear();
    }
    return ret;
}

std::string File::getPath(std::string file)
{
	std::string ret, temp;
	for (unsigned int i = 0; i<file.size(); ++i)
	{
		if (file[i]=='/')
		{
			ret += temp+"/";
			temp.clear();
		}
		else if (file[i]=='.')
			return ret;
		else
			temp.push_back(file[i]);
	}
	return ret;
}
