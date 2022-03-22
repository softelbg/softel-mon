#pragma once

#include <softel/core/intf/IContainable.hpp>

namespace SF {
namespace CORE {

typedef std::vector<std::string> TRow;
typedef std::vector<std::string> TCol;
typedef std::unordered_map<std::string, size_t> TIdx;


class ITable : virtual public IContainable
{
public:
    virtual ~ITable() {}

    // Add a row by an column-ordered vector of values and a column name used to find the key
    virtual void addRow(const TRow& row, const std::string& columnName) = 0;
    
    // Find a column in a row
    virtual std::string& find(const std::string& key, const std::string& col, std::string& value) const = 0;
    // Find a column in a row, but throw when not found.
    virtual const std::string& find(const std::string& key, const std::string& col) const throw() = 0;

    // Get entire column. TODO: Consider iterator based access to columns also...
    virtual bool getColumn(const std::string& keyColumn, TCol& column) const = 0;
    
    // Valid table is a table with non-empty column list.
    virtual bool isValid() const = 0;
};

} /* namespace CORE */
} /* namespace SF */