/*
 * PluginSimInfo.h
 *
 *	Various SIM components could subscribe as information source
 *	Should provide web-based SIM health info from the subscribed components
 *
 *  Created on: Apr 18, 2014
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2014 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef PluginSimInfo_H_
#define PluginSimInfo_H_

#include <softel/core/net/web/BPluginBase.h>
#include <softel/core/commonDefs.h>

namespace SF {
namespace SIM {

class PluginSimInfo: public SF::WEB::BPluginBase {
public:
	PluginSimInfo();
	virtual ~PluginSimInfo();

	virtual void run(const SF::CORE::NET::RequestHttpBase& req);

	void subscribe(SF::CORE::Ptr<IObjectInterface> s);

private:
	std::vector<SF::CORE::Ptr<IObjectInterface> > m_subscribers;
};

} /* namespace SIM */
} /* namespace SF */
#endif /* PluginSimInfo_H_ */
