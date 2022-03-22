#pragma once

namespace SF {
namespace CORE {

class IContainable
{
public:
    virtual ~IContainable() {}

    virtual size_t size() const = 0;
    virtual bool empty() const = 0;
    
    virtual void clear() = 0;
};

} /* namespace CORE */
} /* namespace SF */