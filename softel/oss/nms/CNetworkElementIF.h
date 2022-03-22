/*
 * CNetworkElementIF.h
 *
 *	NEtwork Element Interface - should has ip, mac...
 *
 *  Created on: 21.02.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CNETWORKELEMENTIF_H_
#define CNETWORKELEMENTIF_H_

#include <softel/db/CDataRow.h>
#include "OssNetwork.h"

namespace SF {
namespace OSS {
namespace NMS {

class CNetworkElementIF {
public:
	CNetworkElementIF();
	CNetworkElementIF(const SF::DB::CDataRow& row);
	CNetworkElementIF(const std::string& ip);
	CNetworkElementIF(const std::string& ip, const std::string& mac);
	virtual ~CNetworkElementIF();

	void setIp(const std::string& ip);
	std::string getIp() const;

	void setMac(const std::string& mac);
	std::string getMac() const;

	const std::vector<std::string> getIdxKeyList() const;

	bool isValid();

	void dumpInfo() const;

private:

	SF::DB::CDataRow m_row_params;

};

} /* namespace NMS */
} /* namespace OSS */
} /* namespace SF */

#endif /* CNETWORKELEMENTIF_H_ */
