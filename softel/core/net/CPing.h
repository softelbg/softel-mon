/*
 * CPing.h
 *
 *	Ping Command.
 *	Using shell ping command. Should provide some parsing of the result.
 *
 *  Created on: 11.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CPING_H_
#define CPING_H_

#include <softel/core/system/CShellCmd.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <iostream>
#include <string>
#include <sstream>
#include <map>

namespace SF {
namespace CORE {
namespace NET {

class CPing : public SF::CORE::SYSTEM::CShellCmd {
public:
	CPing(const std::string& ip);
	virtual ~CPing();

	std::string getIp() const {
		return m_ip;
	}

	void setIp(std::string ip) {
		m_ip = ip;
	}

	std::string& getFinalResult();

	virtual void run();

	void dumpInfo();

protected:

	void initPingCmdParams();
	void doPingCmd();

	void parseResult();
	void parsePacketNum(const std::string& line);
	void parsePacketRoundtrip(const std::string& line);

	virtual void setResponse();

	std::string	m_ip;
	std::string	m_response_final;

	int		m_num_packets;
	int		m_packet_size;
	float	m_period_wait;

	int		m_num_sent;
	int		m_num_received;
	float	m_loss;
	float	m_roundtrip_max;
	float	m_roundtrip_avg;

	std::map<std::string, std::string> m_params;


};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */

#endif /* CPING_H_ */
