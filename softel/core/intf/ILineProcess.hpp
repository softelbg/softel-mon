#pragma once

namespace SF {
namespace CORE {

class ILineProcess
{
public:
    virtual ~ILineProcess() {}

    virtual void processLine(const std::string& line) = 0;
};

} /* namespace CORE */
} /* namespace SF */