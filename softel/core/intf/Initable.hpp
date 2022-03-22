#pragma once

namespace SF {
namespace CORE {

class Initable
{
public:
    virtual ~Initable() {}

    virtual void init() = 0;
};

} /* namespace CORE */
} /* namespace SF */