#pragma once

#include <softel/core/intf/Initable.hpp>
#include <softel/core/intf/ILineProcess.hpp>

namespace SF {
namespace CORE {
namespace UTIL {

class TextFile : public virtual ILineProcess, public virtual Initable
{
public:
    explicit TextFile(const std::string& path);
    virtual ~TextFile();

    virtual void init();
private:
    void parseFilePath();

    std::string path_;
};

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */
