/*
 * CSensorTekom.h
 *
 *  Created on: 12.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSENSORTEKOM_H_
#define CSENSORTEKOM_H_

#include <map>
#include <iostream>
#include <stdlib.h>

#include "BSensorBase.h"
#include "BFunctionBase.h"

namespace SF {
namespace OSS {
namespace EXEC {

class CSensorTekom: public BSensorBase {
public:
	CSensorTekom(const std::string& id, const std::string& value);
	virtual ~CSensorTekom();

	std::string getFinalValue();
	std::string getFinalValueFormated();
	std::string getDimension();

	void dumpInfo();

private:
	void initConvertFunction();
	void convert();

	std::string	m_final_value;
	std::string	m_dimension;
	std::string	m_final_value_formated;

	std::map<std::string, BFunctionBase*>	m_list_convert_function;

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* CSENSORTEKOM_H_ */
