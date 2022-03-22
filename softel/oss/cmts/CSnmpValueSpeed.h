/*
 * CSnmpValueSpeed.h
 *
 * 	Read some counter twice with some period between the readings.
 * 	Do the differentiantion by elapsed time - get the speed
 *
 *  Created on: 17.08.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSNMPVALUESPEED_H_
#define CSNMPVALUESPEED_H_

#include "CSnmpValue.h"
#include "CSnmpValueSet.h"
#include <softel/core/util/CTimeCounterFine.h>
#include <softel/core/net/CIp.h>
#include <softel/core/system/CSystemTools.h>

namespace SF {
namespace OSS {
namespace CMTS {

class CSnmpValueSpeed : public CSnmpValue {
public:
	CSnmpValueSpeed(const CSnmpOid& snmpOid);
	virtual ~CSnmpValueSpeed();

	virtual void start();
	virtual std::string& getSpeed();

	void setIpTools(SF::CORE::NET::CIp* pIpTools);
	void setPeriod(int period);

	//void clear();
	//int size();

protected:

	std::string				m_speed;
	int					m_period;

	SF::CORE::UTIL::CTimeCounterFine	m_counter;

	CSnmpValueSet		m_values;

	SF::CORE::NET::CIp*				m_pIpTools;

};

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

#endif /* CSNMPVALUESPEED_H_ */
