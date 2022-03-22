/*
 * CCmtsIfCmCounter.h
 *
 *	CMTS IF -> CM counter.
 *	Should contain at least All CMs and Partial CMs counters.
 *	Calculate Partial/All CMs per IF.
 *
 *	TODO: Consider make more abstract Cmts/IF/CM base counters...
 *
 *  Created on: 05.11.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CCMTSIFCMCOUNTER_H_
#define CCMTSIFCMCOUNTER_H_

#include <softel/core/intf/CDumpableInterface.h>
#include <softel/core/intf/CListInterface.h>
#include "CCmtsIfRawCmCounter.h"

namespace SF {
namespace OSS {
namespace ALERT {

class CCmtsIfCmCounter :
		public SF::CORE::INTF::CListInterface,
		public SF::CORE::INTF::CDumpableInterface {
public:
	CCmtsIfCmCounter(CCmtsIfRawCmCounter* pAll, CCmtsIfRawCmCounter* pPartial);
	virtual ~CCmtsIfCmCounter();

	virtual void clear();
	virtual size_t size() const;
	virtual size_t size(const std::string& cmts_id) const;

	const std::map<std::string, std::map<std::string, double> >& getCalculated() const;
	const std::map<std::string, double>& getCalculated(const std::string& cmts_id) const;
	const double getCalculated(const std::string& cmts_id, const std::string& if_id) const;

	bool getCounters(const std::string& cmts_id, const std::string& if_id, std::map<std::string, int>& result) const;

	void init();

	void dumpInfo();

private:

	CCmtsIfRawCmCounter* m_pAll;
	CCmtsIfRawCmCounter* m_pPartial;

	std::map<std::string, std::map<std::string, double> > m_calculated;

};

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */

#endif /* CCMTSIFCMCOUNTER_H_ */
