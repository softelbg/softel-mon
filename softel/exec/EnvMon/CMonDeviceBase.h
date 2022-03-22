/*
 * CMonDeviceBase.h
 *
 *  Created on: 12.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CMONDEVICEBASE_H_
#define CMONDEVICEBASE_H_

#include "CMonDeviceInterface.h"
#include <softel/core/intf/CRunnableInterface.h>
#include <softel/core/intf/CDumpableInterface.h>
#include <softel/core/net/CHost.h>

namespace SF {
namespace OSS {
namespace EXEC {

class CMonDeviceBase:
		public CMonDeviceInterface,
		public SF::CORE::INTF::CRunnableInterface,
		public SF::CORE::INTF::CDumpableInterface {
public:
	CMonDeviceBase();
	CMonDeviceBase(const SF::CORE::NET::CHost& host);
	virtual ~CMonDeviceBase();

	virtual std::string getResponse();

	void setHost(const SF::CORE::NET::CHost& host);

	const SF::CORE::NET::CHost& getHost() const;

protected:
	void setRawResponse(const std::string& response);

private:
	SF::CORE::NET::CHost	m_host;

	std::string m_raw_response;
};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* CMONDEVICEBASE_H_ */
