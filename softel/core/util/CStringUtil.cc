/*
 * CStringUtil.cc
 *
 *  Created on: 21.02.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CStringUtil.h"

using namespace std;

namespace SF {
namespace CORE {
namespace UTIL {

CStringUtil::CStringUtil() {

}

CStringUtil::~CStringUtil() {

}

/**
 * Tokenize string and fill vector with result tokens.
 */
void CStringUtil::tokenize(const string& tokenizeMe, const string& delim, vector<string>& tokens) {

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << endl;

	tokens.clear();
	size_t pos = 0;
	size_t posPrev = 0;

	while (true) {

		posPrev = pos;

		pos = tokenizeMe.find_first_of(delim, posPrev);

		string subToken = tokenizeMe.substr(posPrev, pos - posPrev);

		tokens.push_back(subToken);

		if (pos != string::npos) {

			pos++;

		} else {
			break;
		}

	}

}

/**
 * Tokenize string and fill vector with result tokens.
 */
void CStringUtil::tokenizeFindSubstring(const string& tokenizeMe, const string& delim, vector<string>& tokens) {

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << endl;

	tokens.clear();
	size_t pos = 0;
	size_t posPrev = 0;

	while (true) {

		posPrev = pos;

		pos = tokenizeMe.find(delim, posPrev);

		string subToken = tokenizeMe.substr(posPrev, pos - posPrev);

		tokens.push_back(subToken);

		if (pos != string::npos) {

			pos += delim.size();

		} else {

			break;

		}

	}

}

/**
 * Replace char in string with other char.
 * Using algorithm::replace...
 */
void CStringUtil::replace(string& inString, char cSearch, char cReplace) {

	std::replace(inString.begin(), inString.end(), cSearch, cReplace);

}

/**
 * Replace substring with string.
 */
void CStringUtil::replace(string& result, const string& searched, const string& replace) {

	vector<string> tokens;

	CStringUtil::tokenizeFindSubstring(result, searched, tokens);

	CStringUtil::implode(tokens, replace, result);

}

void CStringUtil::implode(const vector<string>& tokens, const string& clue, string& result) {

	implode(0, tokens.size(), tokens, clue, result);

}

void CStringUtil::implode(	size_t posStart,
							size_t posEnd,
							const vector<string>& tokens,
							const string& clue,
							string& result) {

	unsigned int i;

	result.clear();

	if (tokens.size() == 0 || posStart < 0 || posEnd < posStart) {
		return;
	}

	if (posEnd > tokens.size()) {
		posEnd = tokens.size();
	}

	for (i = posStart; i < posEnd - 1; i++) {
		result += tokens[i];
		result += clue;
	}

	result += tokens[i];

}

/**
 * Remove char from string.
 * Using algorithm::remove...
 */
void CStringUtil::remove(const char cRemove, string& inString) {

	string::iterator end_pos = std::remove(inString.begin(), inString.end(), cRemove);

	inString.erase(end_pos, inString.end());

}

/**
 * Remove char list from string.
 * Using algorithm::remove...
 */
void CStringUtil::remove(const string& sRemove, string& inString) {
	for(size_t i = 0; i < sRemove.size(); i++) {
		CStringUtil::remove(sRemove[i], inString);
	}
}

/**
 * Find a Substring in the middle of 2 searched strings.
 * Example <<24>> - first = << second = >> result = 24
 */
bool CStringUtil::findFirstMiddleSubstring(const string& tokenizeMe,
		const string& first, const string& second,
		const int posStart, string& result,
		int* pPosFound) {

	size_t posFirst;
	size_t posSecond;

	posFirst = tokenizeMe.find(first, posStart);

	if (posFirst == string::npos) {
		return false;
	}

	posFirst += first.size();

	posSecond = tokenizeMe.find(second, posFirst);

	if (posSecond == string::npos) {
		return false;
	}

	result = tokenizeMe.substr(posFirst, posSecond - posFirst);

	*pPosFound = posSecond + second.size();

	return true;
}

/**
 * Convert MAC to dotted decimal parts.
 * example AA:99:11:BB:CC:12 ->  170.153.17.187....
 */
void CStringUtil::convertMac2DottedDecimal(const string& mac, const string& delim, string& result) {

	size_t i;
	int m;

	stringstream ss;
	vector<string> tokensMac;

	CStringUtil::tokenizeFindSubstring(mac, delim, tokensMac);

	for(i = 0; i < tokensMac.size(); i++) {
		sscanf(tokensMac[i].c_str(), "%x", &m);
		ss << "." << m;
	}

	result += ss.str();
}

/**
 * Convert MAC to decimal number.
 */
void CStringUtil::convertMac2Dec(const string& inputMac, string& result) {

	int HexInt;
	char decMac[32];
	double cm_mac_f;

	string mac = inputMac;

	CStringUtil::remove(" :.", mac);

	strncpy(decMac, mac.c_str(), 6);
	decMac[6] = 0;

	sscanf(decMac, "%x", &HexInt);
	cm_mac_f = HexInt * 16777216.0;
	strncpy(decMac, mac.c_str() + 6, 6);
	decMac[6] = 0;
	sscanf(decMac, "%x", &HexInt);
	cm_mac_f += HexInt;
	sprintf(decMac, "%1.0f", cm_mac_f);

	result = decMac;

}

/**
 * Convert Decimal MAC id to Hex MAC
 */
void CStringUtil::convertDecimal2Mac(const string& mac_decimal, const string& delim, string& mac_hex) {

	CStringUtil::convert2Base(mac_decimal, 16, mac_hex);

	CStringUtil::formatMac(":", mac_hex);

}

/**
 * Split the Input String into tokens with sizeToken.
 */
void CStringUtil::split(const string& input, const int sizeToken, vector<string>& tokens) {

	size_t i = 0;
	int j = 0;

	tokens.clear();

	string token;

	for(i = 0, j = sizeToken; i < input.size(); i++) {

		token += input[i];

		j--;
		if (j == 0) {
			j = sizeToken;
			tokens.push_back(token);
			token.clear();
		}

	}

}

/**
 * Format the input Hex MAC - add delimiter, to upper case, add last zeros if they are not present.
 */
void CStringUtil::formatMac(const string& delim, string& mac) {

	CStringUtil::remove(":.- ", mac);

	string zeros;

	if (mac.size() < 0 || mac.size() > 12) {
		return;
	}

	for(size_t i = 0; i < 12 - mac.size(); i++) {
		zeros = "0" + zeros;
	}

	mac = zeros + mac;

	vector<string> tokensMac;

	CStringUtil::split(mac, 2, tokensMac);
	CStringUtil::implode(tokensMac, delim, mac);

}

/**
 * Convert gps coordinate in a string like 23°17'9.22
 * Output should be a double representation of the coordinate
 */
double CStringUtil::convertGpsCoordinate(const string& inCoordinate) {

	double result = 0;

	string coordinate = inCoordinate;

	CStringUtil::remove("\" NÑEÈ", coordinate);

	vector<string> tokensMajor;
	CStringUtil::tokenize(coordinate, "°", tokensMajor);

	if (tokensMajor.size() == 2) {

		double major = atof(tokensMajor[0].c_str());

		if (tokensMajor[1].size() > 0) {

			vector<string> tokensMinor;
			CStringUtil::tokenize(tokensMajor[1], "\'", tokensMinor);

			if (tokensMinor.size() >= 2) {

				double minutes = atof(tokensMinor[0].c_str());
				double seconds = atof(tokensMinor[1].c_str());

				result = major + (minutes / 60) + (seconds / 3600);

			} else {
				result = major;
			}

		} else {
			result = major;
		}

	} else {
		result = atof(coordinate.c_str());
	}

	return result;
}

/**
 * Convert decimal number to number(base).
 * Currently base should be >= 2 and <= 16 (mostly needed for hex conversion).
 */
void CStringUtil::convert2Base(const string& decimal, const int base,
		string& result) {

	string digits = "0123456789ABCDEF";

	result.clear();

	if (base < 2 || base > 17) {
		return;
	}

	double dInputDecimal = atof(decimal.c_str());
	long long lInputDecimal = (long long) dInputDecimal;

	// Add Sign
	if (dInputDecimal < 0) {
		result = "-" + result;
	}

	do {

		result = digits[lInputDecimal % base] + result;

		lInputDecimal = lInputDecimal / base;

	} while(lInputDecimal != 0);

}

/**
 * Convert double to string
 */
string CStringUtil::fromDouble(double val) {
	stringstream ss;
	ss << val;
	return ss.str();
}
/**
 * Convert double represented in a String to Double.
 * TODO: Now using atof, but consider better implementation...
 */
double CStringUtil::toDouble(const string& val) {
	return atof(val.c_str());
}
/**
 * Convert int to string
 */
string CStringUtil::fromInt(int val) {
	stringstream ss;
	ss << val;
	return ss.str();
}
/**
 * Convert int represented in a String to int.
 * TODO: Now using atoi, but consider better implementation...
 */
int CStringUtil::toInt(const string& val) {
	return atoi(val.c_str());
}

/**
 * Compute Java-style hash of input string.
 * TODO: Implementation pending...
 */
std::string CStringUtil::toJavaHash(const std::string& inStr) {
	return inStr;
}

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */



