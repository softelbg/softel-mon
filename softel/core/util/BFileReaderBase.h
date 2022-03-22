/*
 * BFileReaderBase.h
 *
 *	Base File Reader
 *	TODO: Consider inherit Abstract base Reader
 *
 *  Created on: 18.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef BFILEREADERBASE_H_
#define BFILEREADERBASE_H_

#include <iostream>
#include <fstream>
#include <string>

#include "BFileBase.h"
#include <softel/core/intf/IInitableInterface.h>

namespace SF {
namespace CORE {
namespace UTIL {

class BFileReaderBase : public BFileBase, public SF::CORE::INTF::IInitableInterface {
public:
	BFileReaderBase();
	BFileReaderBase(const std::string& fileName);
	virtual ~BFileReaderBase();

	virtual void read();
	virtual void write();

	virtual void init();

protected:
	virtual void processLine(const std::string& line)=0;

};

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */

#endif /* BFILEREADERBASE_H_ */
