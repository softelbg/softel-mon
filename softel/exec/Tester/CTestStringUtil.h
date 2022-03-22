/*
 * CTestStringUtil.h
 *
 *  Created on: 14.01.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CTESTSTRINGUTIL_H_
#define CTESTSTRINGUTIL_H_

#include <string>

#include "CTestCase.h"

namespace SF {
namespace OSS {
namespace EXEC {

class CTestStringUtil: public CTestCase {
public:
	CTestStringUtil();
	virtual ~CTestStringUtil();

	virtual void run();

	void dumpInfo();

private:

	std::string m_dec_mac;
	std::string m_dec_mac_mark;
	std::string m_hex_mac;
	std::string m_hex_mac_mark;

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* CTESTSTRINGUTIL_H_ */
