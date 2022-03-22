/*
 * BFileBase.h
 *
 *	Base FILE
 *
 *  Created on: 03.04.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef BFILEBASE_H_
#define BFILEBASE_H_

#include <iostream>
#include <fstream>
#include <string>

#include <softel/core/intf/IReaderWriterInterface.h>
#include <softel/core/intf/IOpenCloseInterface.h>

namespace SF {
namespace CORE {
namespace UTIL {

class BFileBase : public std::fstream, public SF::CORE::INTF::IReaderWriterInterface {
public:
	BFileBase();
	BFileBase(const std::string& fileName);
	virtual ~BFileBase();

	const std::string& getFileName() const;
	void setFileName(const std::string& fileName);

	void setMode(const ios_base::openmode& mode);

	void openFile();
	void closeFile();

	virtual void read();
	virtual void read(std::string result);

	virtual void write(const std::string& input);
	virtual void write();

private:
	ios_base::openmode 		m_mode;
	std::string 			m_file_name;
};

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */

#endif /* BFILEBASE_H_ */
