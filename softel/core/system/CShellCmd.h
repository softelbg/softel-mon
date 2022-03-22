/*
 * CShellCmd.h
 *
 *	Base Shell command via pipe
 *
 *  Created on: 11.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSHELLCMD_H_
#define CSHELLCMD_H_

#include <string>

#include <softel/core/CObject.h>

namespace SF {
namespace CORE {
namespace SYSTEM {

class CShellCmd : public SF::CORE::CObject {
public:
	CShellCmd();
	CShellCmd(const std::string& cmd);
	virtual ~CShellCmd();

	const std::string& getCmd() const {
		return this->m_cmd;
	}

	void setCmd(const std::string& cmd) {
		this->m_cmd = cmd;
	}

	const std::string& getResult() const {
		return m_result;
	}

	virtual void run();

protected:
	void execute();

private:
	std::string m_cmd;
	std::string	m_result;
};

} /* namespace SYSTEM */
} /* namespace CORE */
} /* namespace SF */

#endif /* CSHELLCMD_H_ */
