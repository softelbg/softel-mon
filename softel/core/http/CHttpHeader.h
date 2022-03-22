/*
 * CHttpHeader.h
 *
 *	HTTP Header. Should has a map with header properties.
 *
 *  Created on: 20.05.2012
 *      Author: STAN
 *
 *		www.softel.bg
 *
 *		Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CHTTPHEADER_H_
#define CHTTPHEADER_H_

#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <string>

namespace SF {
namespace CORE {
namespace HTTP {

class CHttpHeader {

public:

	CHttpHeader();
	CHttpHeader(const std::string& header);

	void setHeader(const std::string& header);

	std::string& getHeader();

private:

	void parseHeader();

	std::string m_header;

};

} /* namespace HTTP */
} /* namespace CORE */
} /* namespace SF */

#endif /* CHTTPHEADER_H_ */
