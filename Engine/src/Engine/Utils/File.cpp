/**
* @Author   Guillaume Labey
*/

#include <fstream>
#include <vector>

#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/Logger.hpp>

#include <Engine/Utils/File.hpp>

File::File() {}

File::File(const std::string& path)
{
    setPath(path);
}

File::~File() {}

bool    File::loadFromFile(const std::string &fileName)
{
    std::ifstream               file;
    int                         fileSize;
    std::vector<char>           fileContent;

    LOG_INFO("Loading file \"%s\"", fileName.c_str());
    file.open(fileName.c_str());
    if (!file.good())
        EXCEPT(FileNotFoundException, "Failed to open file \"%s\"", fileName.c_str());

    file.seekg(0, file.end);
    fileSize = static_cast<int>(file.tellg());
    file.seekg(0, file.beg);

    fileContent.resize(fileSize);
    file.read(fileContent.data(), fileSize);
    file.close();

    _content = std::string(fileContent.begin(), fileContent.end());

    return (true);
}

void    File::save()
{
    std::ofstream file;

    LOG_INFO("Saving file \"%s\"", getPath().c_str());
    file.open(getPath().c_str(), std::ios::trunc);
    if (!file.good())
        EXCEPT(FileNotFoundException, "Failed to open file \"%s\"", getPath().c_str());

    file << _content;
    file.close();
}

const std::string&  File::getContent() const
{
    return (_content);
}

void    File::setContent(const std::string& content)
{
    _content = content;
}
