#include <softel/core/common.h>
#include <softel/core/commonDefs.h>
#include "TableBase.h"


namespace CN = SF::CORE::UTIL;

CN::TableBase::TableBase(const TRow& columnNames)
{
    init(columnNames);
}

CN::TableBase::~TableBase()
{
}

void
CN::TableBase::init(const TRow& columnNames)
{
    init(columnNames.begin(), columnNames.end());
}

void
CN::TableBase::addRow(const TRow& row, const std::string& columnName)
{
    size_t posColumn = 0;
    if (getColumnPosition(columnName, posColumn)) {
        idx_primary_[row[posColumn]] = table_.size();
        table_.push_back(row);
    }
}

bool
CN::TableBase::findRow(const std::string& key, CRow& row) const
{
    bool isFound = false;

    auto itIdx = idx_primary_.find(key);
    if (itIdx != idx_primary_.end()) {
        row = CRow(table_[itIdx->second], idx_col_);
        isFound = true;
    }

    return isFound;
}

bool CN::TableBase::getRow(size_t pos, CRow& row) const
{
    bool isFound = false;

    if (pos >= 0 && pos < table_.size()) {
        isFound = true;
        row = CRow(table_[pos], idx_col_);
    }

    return isFound;
}

std::string&
CN::TableBase::find(const std::string& key, const std::string& col, std::string& value) const
{
    CRow row;

    if (findRow(key, row)) {
        row.find(col, value);
    }

    return value;
}

const std::string&
CN::TableBase::find(const std::string& key, const std::string& col) const throw()
{
    auto itPrim = idx_primary_.find(key);
    auto itCol = idx_col_.find(col);

    if (itPrim != idx_primary_.end() && itCol != idx_col_.end()) {
        return (table_[itPrim->second])[itCol->second];
    }
    else {
        throw std::exception(/*"Not Found key = " + key + " col = " + col*/);
    }
}


bool
CN::TableBase::getColumn(const std::string& keyColumn, TCol& column) const
{
    column.clear();
    column.reserve(size());

    bool isFound = false;

    auto itPosColumn = idx_col_.find(keyColumn);

    if (itPosColumn != idx_col_.end()) {
        for (auto itRow = table_.begin(); itRow != table_.end(); ++itRow) {
            column.push_back((*itRow)[itPosColumn->second]);
        }

        isFound = true;
    }
 
    return isFound;
}

bool
CN::TableBase::getColumnPosition(const std::string& keyColumn, size_t& pos)
{
    bool isFound = false;

    auto itPosColumn = idx_col_.find(keyColumn);

    if (itPosColumn != idx_col_.end()) {
        pos = itPosColumn->second;
        isFound = true;
    }

    return isFound;
}

size_t
CN::TableBase::size() const {
    return table_.size();
}

bool
CN::TableBase::empty() const
{
    return table_.empty();
}

void
CN::TableBase::clear() {
    table_.clear();
    idx_col_.clear();
    idx_primary_.clear();
}

bool
CN::TableBase::isValid() const
{
    // Validation of table is on column presence.
    return !idx_col_.empty();
}

std::string&
CN::CRow::find(const std::string& col, std::string& value) const
{
    value.clear();

    auto itCol = idx_.find(col);
    if (itCol != idx_.end()) {
        value = row_[itCol->second];
    }

    return value;
}

std::string
CN::CRow::find(const std::string& col) const
{
    std::string result;
    return find(col, result);
}
