
#include <softel/core/common.h>
#include "TextFile.h"
#include "Options.h"

namespace CN = SF::CORE::UTIL;

CN::TextFile::TextFile(const std::string& path)
    : path_(path)
{
}

CN::TextFile::~TextFile()
{
}

void
CN::TextFile::init()
{
    std::string line;

    parseFilePath();

    std::ifstream fileIfStream(path_.c_str());

    if (fileIfStream.is_open())
    {
        while (fileIfStream.good())
        {
            std::getline(fileIfStream, line);
            processLine(line);
        }
    }
    else
    {
        std::cout << __FUNCTION__ << " Error opening file " << path_ << std::endl;
    }
}

void CN::TextFile::parseFilePath()
{
    if (path_.find('\\') == std::string::npos && path_.find('/') == std::string::npos) {
        path_ = Options::getBasePath() + "\\" + path_;
    }
}
