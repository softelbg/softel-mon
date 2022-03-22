#pragma once

#include <softel/core/intf/ITable.hpp>

namespace SF {
namespace CORE {
namespace UTIL {


// Single Table Row 
class CRow
{
public:
    CRow() {}
    CRow(const TRow& row, const TIdx& idx) : row_(row), idx_(idx) {}

    std::string find(const std::string& col) const;
    std::string& find(const std::string& col, std::string& value) const;

private:
    TRow row_;
    TIdx idx_;
};


// The Base Table
class TableBase : virtual public ITable
{
public:
    TableBase() {}
    explicit TableBase(const TRow& columnNames);
    virtual ~TableBase();

    template <class IT> void init(IT begin, IT end) {
        clear();
        int i = 0;
        // Build column index
        for (IT it = begin; it != end; ++it, ++i) {
            idx_col_[*it] = i;
        }
    }

    template <class IT> void addRow(IT begin, IT end, const std::string& columnName) {
        addRow(TRow(begin, end), columnName);
    }

    // Init table as defining its columns
    void init(const TRow& columnNames);
    // Add a row by an column-ordered vector of values and a column name used to find the key
    virtual void addRow(const TRow& row, const std::string& columnName);

    // Find a row using primary index
    virtual bool findRow(const std::string& key, CRow& row) const;
    virtual bool getRow(size_t pos, CRow& row) const;

    virtual std::string& find(const std::string& key, const std::string& col, std::string& value) const;
    virtual const std::string& find(const std::string& key, const std::string& col) const throw();

    virtual bool getColumn(const std::string& keyColumn, TCol& column) const;

    virtual bool isValid() const;
    virtual size_t size() const;
    virtual bool empty() const;
    virtual void clear();

protected:
    // Get column position index by its name.
    bool getColumnPosition(const std::string& keyColumn, size_t& pos);

    std::vector<TRow> table_; // A Row container
    TIdx idx_col_; // Column index
    TIdx idx_primary_; // Single primary row index.
    //std::vector<TIdx> idx_; // TODO: Consider indexing on every column if needed....
};


} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */

