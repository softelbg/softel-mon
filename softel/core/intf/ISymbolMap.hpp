#pragma once

#include "IContainable.hpp"
#include <softel/core/util/TableBase.h>

namespace SF {
namespace CORE {

class ISymbolMap :  public virtual IContainable
{
public:
    typedef std::map<std::string, std::string> TSymbol;

    virtual ~ISymbolMap() {}

    // Find a row searching the source symbol.
    virtual bool findRow(const std::string& sourceSym, SF::CORE::UTIL::CRow& row) const = 0;
    virtual const SF::CORE::UTIL::TableBase& getTable() const = 0;
};

}
}
