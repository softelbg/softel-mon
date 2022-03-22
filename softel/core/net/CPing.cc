/*
 * CPing.cc
 *
 *  Created on: 11.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CPing.h"

#include <softel/core/util/CStringUtil.h>

using namespace std;
using namespace SF::CORE::UTIL;

namespace SF {
namespace CORE {
namespace NET {

#define CMD_STR_PING	"ping -c 10 -s 1472 -i 0.1 "

#define MSG_LOSS		"Ping Losses - "
#define MSG_DELAY		"RoundTrip too long - "

CPing::CPing(const std::string& ip) {

	this->setIp(ip);

	this->initPingCmdParams();

	this->doPingCmd();

}

CPing::~CPing() {
	// TODO Auto-generated destructor stub
}

void CPing::initPingCmdParams() {

	this->m_num_packets = 10;
	this->m_packet_size = 1472;
	this->m_period_wait = 0.1;

	this->m_num_received = 0;
	this->m_loss = 0;
	this->m_roundtrip_max = 0;
	this->m_roundtrip_avg = 0;

}

/**
 * Build the shell command line.
 */
void CPing::doPingCmd() {

	std::stringstream cmd;

	cmd << "ping ";
	cmd << "-c " << this->m_num_packets << " ";
	cmd << "-s " << this->m_packet_size << " ";
	cmd << "-i " << this->m_period_wait << " ";
	cmd << this->m_ip;

	this->setCmd(cmd.str());
}

/**
 * Run the Ping.
 * Should do the ping shell command first.
 * Parse the raw result.
 */
void CPing::run() {

	this->doPingCmd();

	this->execute();

	this->parseResult();

}

/**
 * Parse Result from Ping command.
 * Should set delay and number of lost packets.
 *
 *	TODO: Consider rework and make condition making functionality...
 */
void CPing::parseResult() {

	vector<std::string> listLines;

	CStringUtil::tokenize(getResult(), "\n", listLines);

	for (int i = 0; i < listLines.size(); i++) {
		this->parsePacketNum(listLines[i]);
		this->parsePacketRoundtrip(listLines[i]);
	}

	this->setResponse();

}

/**
 * Parse the packet loss.
 */
void CPing::parsePacketNum(const std::string& line) {

	vector<std::string> listTokens;

	CStringUtil::tokenizeFindSubstring(line, ", ", listTokens);

	if (listTokens.size() == 3) {
		for (int i = 0; i < listTokens.size(); i++) {
			//cout << "\ttoken " << i << " : " << listTokens[i] << endl;

			vector<std::string> listParamTokens;

			CStringUtil::tokenize(listTokens[i], " ", listParamTokens);

			if (listParamTokens.size() == 3) {
				CStringUtil::remove('%', listParamTokens[0]);
				this->m_params[listParamTokens[2]] = listParamTokens[0];
			}
		}

		this->m_loss = atof(m_params["loss"].c_str());
		this->m_num_sent = atoi(m_params["transmitted"].c_str());
		this->m_num_received = atoi(m_params["received"].c_str());
	}

}

/**
 * Parse packet round trips.
 */
void CPing::parsePacketRoundtrip(const std::string& line) {

	vector<std::string> listTokens;

	CStringUtil::tokenize(line, "/", listTokens);

	if (listTokens.size() == 7) {
		m_roundtrip_avg = atof(listTokens[4].c_str());
		m_roundtrip_max = atof(listTokens[5].c_str());
	}

}

std::string& CPing::getFinalResult() {
	return this->m_response_final;
}

void CPing::dumpInfo() {

	cout << "Ping Cmd: " << this->getCmd() << endl;
	cout << "Raw Result: " << endl << this->getResult() << endl << endl;
	cout << "Final Result: " << endl << this->getFinalResult() << endl << endl;

	cout << "m_num_sent: " << m_num_sent << endl;
	cout << "m_num_received: " << m_num_received << endl;
	cout << "m_lost: " << m_loss << endl;
	cout << "m_roundtrip_max: " << m_roundtrip_max << endl;
	cout << "m_roundtrip_avg: " << m_roundtrip_avg << endl;

}

void CPing::setResponse() {

	this->m_response_final = getResult();

}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */

