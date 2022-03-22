/*
 * IReaderWriterInterface.h
 *
 *  Created on: 03.04.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef IREADERWRITERINTERFACE_H_
#define IREADERWRITERINTERFACE_H_

#include <string>

namespace SF {
namespace CORE {
namespace INTF {

class IReaderWriterInterface {
public:
	IReaderWriterInterface();
	virtual ~IReaderWriterInterface();

	virtual void read()=0;
	virtual void read(std::string result)=0;

	virtual void write()=0;
	virtual void write(const std::string& input)=0;

};

} /* namespace INTF */
} /* namespace CORE */
} /* namespace SF */

#endif /* IREADERWRITERINTERFACE_H_ */
