/*
 * BSensorBase.h
 *
 *  Created on: 12.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef BSENSORBASE_H_
#define BSENSORBASE_H_

#include <string>

namespace SF {
namespace OSS {
namespace EXEC {

class BSensorBase {
public:
	BSensorBase();
	BSensorBase(const std::string& id, const std::string& value);
	virtual ~BSensorBase();

	virtual void setId(const std::string& id);
	virtual void setValue(const std::string& value);

	virtual const std::string getValue() const;
	virtual const std::string getId() const;

private:
	std::string	m_id;
	std::string	m_value;

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* BSENSORBASE_H_ */
