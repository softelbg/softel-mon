/*
 * CThresholdListRaw.h
 *
 *	Raw Threshold List. Contains a Data Set filled from DB.
 *
 *
 *  Created on: 02.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CTHRESHOLDLISTRAW_H_
#define CTHRESHOLDLISTRAW_H_

#include "CSqlThresholdTools.h"

#include <softel/db/CDataSet.h>

namespace SF {
namespace OSS {
namespace ALERT {

class CThresholdListRaw {
public:
	CThresholdListRaw();
	virtual ~CThresholdListRaw();

	void clear();
	int size() const;

	const SF::DB::CDataSet& getRawDataSet() const;

	virtual void init();

	virtual void dumpInfo() const;

protected:

	SF::DB::CDataSet m_data_set;

};

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */

#endif /* CTHRESHOLDLISTRAW_H_ */
