/*
 * CServerInfo.h
 *
 *  Created on: 30.01.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSERVERINFO_H_
#define CSERVERINFO_H_

#include <vector>
#include <string>

#include <softel/core/system/CShellCmd.h>

namespace SF {
namespace OSS {
namespace EXEC {

class CServerInfo {
public:
	CServerInfo();
	virtual ~CServerInfo();

	void init();
	void executeList();

	std::string getResult(int pos) const;
	std::string getResult() const;

	void clear();
	int size() const;

private:

	bool validatePos(int pos) const;
	void initCmdList();

	std::vector<SF::CORE::SYSTEM::CShellCmd> m_cmd_list;

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* CSERVERINFO_H_ */
