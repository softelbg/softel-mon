/*
 * BDemonizeBase.h
 *
 *	Base Demonizer.
 *	Should make an initial process run as a daemon with a suplemental process
 *	which will fork and run again the child...
 *
 *  Created on: Apr 6, 2014
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef BDEMONIZEBASE_H_
#define BDEMONIZEBASE_H_

#include <softel/core/CObject.h>

#include <softel/core/error/CErrorCommon.h>
#include <softel/core/error/CErrorSql.h>
#include <softel/core/system/CSystemTools.h>

#include <iostream>
#include <string>

namespace SF {
namespace CORE {
namespace SYSTEM {

class BDemonizeBase: public SF::CORE::CObject {
public:
	BDemonizeBase();
	virtual ~BDemonizeBase();

	virtual void runDemonizedServer() = 0;

	void start();

};

} /* namespace SYSTEM */
} /* namespace CORE */
} /* namespace SF */
#endif /* BDEMONIZEBASE_H_ */
