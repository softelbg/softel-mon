/*
 * CStringUtil.h
 *
 *  Created on: 21.02.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSTRINGUTIL_H_
#define CSTRINGUTIL_H_

#include <stdio.h>

#include <vector>
#include <string>
#include <cstring>
#include <algorithm>

#include <iostream>
#include <sstream>

namespace SF {
namespace CORE {
namespace UTIL {

class CStringUtil {

public:

	CStringUtil();
	~CStringUtil();

	static void tokenize(const std::string& tokenizeMe, const std::string& delim, std::vector<std::string>& tokens);
	static void tokenizeFindSubstring(const std::string& tokenizeMe, const std::string& delim, std::vector<std::string>& tokens);

	static bool findFirstMiddleSubstring(	const std::string& tokenizeMe,
											const std::string& first,
											const std::string& second,
											const int posStart,
											std::string& result,
											int* pPosFound);

	static void replace(std::string&, char, char);
	static void replace(std::string&, const std::string&, const std::string&);

	static void implode(const std::vector<std::string>& tokens, const std::string& clue, std::string& result);
	static void implode(size_t posStart, size_t posEnd, const std::vector<std::string>& tokens, const std::string& clue, std::string& result);

	static void remove(const char, std::string&);
	static void remove(const std::string&, std::string&);

	static void split(const std::string&, const int sizeToken, std::vector<std::string>& tokens);

	static void convertMac2Dec(const std::string& mac, std::string& decMac);
	static void convertMac2DottedDecimal(const std::string&mac, const std::string& delim, std::string& result);
	static void convertDecimal2Mac(const std::string& mac_decimal, const std::string& delim, std::string& mac_hex);
	static void formatMac(const std::string& delim, std::string& mac);

	static double convertGpsCoordinate(const std::string& coordinate);

	static void convert2Base(const std::string& decimal, const int base, std::string& result);

	static std::string fromDouble(double val);
	static double toDouble(const std::string& val);
	static std::string fromInt(int val);
	static int toInt(const std::string& val);

	static std::string toJavaHash(const std::string& inStr);

private:

};

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */

#endif /* CSTRINGUTIL_H_ */
