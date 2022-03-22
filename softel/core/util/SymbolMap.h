#pragma once

#include "TextFile.h"
#include <softel/core/intf/ISymbolMap.hpp>


namespace SF {
namespace CORE {
namespace UTIL {

// Symbol Map Column names definitions, these are in the first line of the symbol file.
static const std::string SymSourceSymbol =              "CurrencyPair";

/*
 *  Symbol Mapping Config File.
 *  Reads the symbol map file and build a table.
 *
 *  Config File format example:
 *
    SourceSymbol;Symbol;PriceExponent;Exchange;SubscriptionType;SecurityType;Level;StateMonitoringEnabled;StateMonitoringTimeout
    USDRUBSAXO;USDRUB;0;240;A;20;;TRUE;10
    STJ;STJ.UK;0;151;A;1;;TRUE;10
    A2A;A2.IT;0;223;A;1;;TRUE;10
    AAL;AAL.UK;0;151;A;1;;TRUE;10
    ABF;ABF.UK;0;151;A;1;;TRUE;10
 *
 *
 */
class SymbolMap :   virtual public TextFile, virtual public ISymbolMap
{
public:
    explicit SymbolMap(const std::string& path);
    virtual ~SymbolMap();

    size_t size() const;
    bool empty() const;
    void clear();

    virtual bool findRow(const std::string& sourceSym, CRow& row) const;

	virtual void init();

    const TableBase& getTable() const {
        return table_;
    }

private:
    void processLine(const std::string& line);

    TableBase table_;
};


}
}
}

