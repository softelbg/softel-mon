/*
 * BFileIniBase.h
 *
 *	Base INI File. Should read a file and read all params into data row.
 *
 *  Created on: 29.03.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef BFILEINIBASE_H_
#define BFILEINIBASE_H_

#include "BFileReaderBase.h"
#include <softel/db/CDataRow.h>
#include "CStringUtil.h"

namespace SF {
namespace CORE {
namespace UTIL {

class BFileIniBase: public BFileReaderBase {
public:
	BFileIniBase(const std::string& fileName);
	virtual ~BFileIniBase();

	std::string get(const std::string& key) const;
	std::string get(const std::string& section, const std::string& key) const;

	void dumpInfo() const;

protected:
	virtual void processLine(const std::string& line);

private:

	void checkSection(const std::string& line);
	void processParamLine(const std::string& line);
	std::string doKey(const std::string& section, const std::string& key) const;

	SF::DB::CDataRow m_row_params;

	std::vector<std::string> m_list_sections;

};

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */

#endif /* BFILEINIBASE_H_ */
