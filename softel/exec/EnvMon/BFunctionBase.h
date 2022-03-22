/*
 * BFunctionBase.h
 *
 *	Base Function converter. Should implement some basic functions.
 *	At least should handle simple linear conversions between 2 linear Set
 *
 *  Created on: 15.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef BFUNCTIONBASE_H_
#define BFUNCTIONBASE_H_

#include <string>
#include <sstream>

namespace SF {
namespace OSS {
namespace EXEC {

class BFunctionBase {
public:
	BFunctionBase();
	BFunctionBase(	const double a1, const double b1, const int size1,
					const double a2, const double b2, const int size2, const std::string& dimension2);
	virtual ~BFunctionBase();

	void setLinear(	const double a1, const double b1, const int size1,
					const double a2, const double b2, const int size2, const std::string& dimension2);

	std::string getLinearValueStr();
	std::string getLinearValueFormatedStr();
	std::string getLinearDimension();

	double convert(double x);

private:
	double	m_linear_a1;
	double	m_linear_b1;
	int		m_linear_size1;
	double	m_linear_a2;
	double	m_linear_b2;
	int		m_linear_size2;
	std::string	m_linear_dimension2;

	double m_last_result;


};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* BFUNCTIONBASE_H_ */
