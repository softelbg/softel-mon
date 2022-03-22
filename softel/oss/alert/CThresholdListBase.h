/*
 * CThresholdListBase.h
 *
 *	Base List of Thresholds.
 *
 *  Created on: 03.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CTHRESHOLDLISTBASE_H_
#define CTHRESHOLDLISTBASE_H_

#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>

#include "CThreshold.h"
#include "CSqlThresholdTools.h"

#include <softel/db/CDataSet.h>
#include <softel/db/CDataRow.h>

namespace SF {
namespace OSS {
namespace ALERT {

class CThresholdListBase {
public:
	CThresholdListBase();
	virtual ~CThresholdListBase();

	void clear();
	int size() const;

	virtual void init()=0;

	void add(const SF::DB::CDataRow& dr);

	virtual void dumpInfo() const;

protected:

	std::vector<CThreshold>	m_list_thresholds;

	SF::DB::CDataSet	m_data_set;

};

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */

#endif /* CTHRESHOLDLISTBASE_H_ */
