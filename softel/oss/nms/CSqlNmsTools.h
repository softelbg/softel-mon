/*
 * CSqlNmsTools.h
 *
 *  Created on: 03.03.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSQLNMSTOOLS_H_
#define CSQLNMSTOOLS_H_

#include <softel/oss/cmts/CSqlToolsBase.h>
#include <softel/db/CDataSet.h>

namespace SF {
namespace OSS {
namespace NMS {

class CSqlNmsTools: public SF::OSS::CMTS::CSqlToolsBase {
public:
	CSqlNmsTools();
	virtual ~CSqlNmsTools();

	void selectListNetElem(const std::string& filter, SF::DB::CDataSet& ds);
	void selectListNetElemUPS(SF::DB::CDataSet& ds);

	void selectListSnmpOid(const std::string& from, SF::DB::CDataSet& ds);
	void selectListSnmpOidNetElemBase(SF::DB::CDataSet& ds);
	void selectListSnmpOidNetElemUps(SF::DB::CDataSet& ds);

};

} /* namespace NMS */
} /* namespace OSS */
} /* namespace SF */

#endif /* CSQLNMSTOOLS_H_ */
