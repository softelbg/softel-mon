/*
 * CMonDeviceHttpBlizoo.h
 *
 *  Created on: 12.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CMONDEVICEHTTPBLIZOO_H_
#define CMONDEVICEHTTPBLIZOO_H_

#include <map>
#include <string>

#include "CMonDeviceBase.h"
#include "BSensorBase.h"
#include "CSensorTekom.h"
#include <softel/core/net/CHost.h>

namespace SF {
namespace OSS {
namespace EXEC {

class CMonDeviceHttpBlizoo: public CMonDeviceBase {
public:
	CMonDeviceHttpBlizoo(const SF::CORE::NET::CHost& host);
	CMonDeviceHttpBlizoo(const SF::CORE::NET::CHost& host, const std::string& url);
	virtual ~CMonDeviceHttpBlizoo();

	void clear();
	int size() const;

	void sendRequest();

	void setUrl(const std::string& url);

	std::string getResult();

	void run();

	void dumpInfo();

private:

	void parseResult();
	void parseResultLine(const std::string& line);

	void sendHttpRequest();

	std::string	m_url;
	std::string m_result;

	std::map<std::string, CSensorTekom*>	m_sensor_array;

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* CMONDEVICEHTTPBLIZOO_H_ */
