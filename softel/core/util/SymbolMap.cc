
#include <softel/core/common.h>
#include <softel/core/commonDefs.h>

#include "SymbolMap.h"
#include "Options.h"
#include "TableBase.h"

namespace CN = SF::CORE::UTIL;

#define DELIMITERS "=;,"

CN::SymbolMap::SymbolMap(const std::string& path)
    : TextFile(path)
{
    ILOG << __FUNCTION__ << " using " << path << std::endl;
}

CN::SymbolMap::~SymbolMap()
{
}

size_t CN::SymbolMap::size() const
{
    return table_.size();
}

bool CN::SymbolMap::empty() const
{
    return table_.empty();
}

void CN::SymbolMap::clear()
{
    table_.clear();
}

void CN::SymbolMap::init()
{
	table_.clear();
	TextFile::init();
}

void
CN::SymbolMap::processLine(const std::string& line)
{
    // Check for comments
    size_t pos = line.find("//");
    std::string lineNoComments = line.substr(0, pos);
    boost::algorithm::trim(lineNoComments);
    // Check for empty line
    if (lineNoComments.empty()) {
        return;
    }
    
    boost::char_separator<char> sep(DELIMITERS, "", boost::keep_empty_tokens);
    boost::tokenizer<boost::char_separator<char> > tokens(lineNoComments, sep);

    if (!table_.isValid()) {
        // Fill the column names from the first line of the symbol config
        table_.init(tokens.begin(), tokens.end());
    } else {
        // Fill symbol rows and set the symbol index key
        table_.addRow(tokens.begin(), tokens.end(), "CurrencyPair");
    }
}

bool
CN::SymbolMap::findRow(const std::string& sourceSym, CRow& row) const
{
    return table_.findRow(sourceSym, row);
}

