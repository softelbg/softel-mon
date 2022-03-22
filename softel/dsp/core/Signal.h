/*
 * Signal.h
 *
 *  Signal representation from DSP perspective.
 *
 *
 *  Created on: May 6, 2015
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2015 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef SIGNAL_H_
#define SIGNAL_H_

#include <softel/core/CObject.h>

#include <vector>
#include <sstream>

namespace SF {
namespace DSP {

template <class T>
class Signal: public SF::CORE::CObject {
public:
	Signal(const std::vector<T>& time_domain, int sampling_rate) :
		m_time_domain(time_domain), m_sampling_rate(sampling_rate) {}

	virtual ~Signal() {}

	size_t size() const {
		return m_time_domain.size();
	}

	std::string toString() {
		std::ostringstream ss;

		ss << "Signal sampling rate: " << m_sampling_rate << " ";
		for(size_t i = 0; i < m_time_domain.size(); ++i) {
			ss << "s[" << i << "] = " << m_time_domain[i] << " ";
		}

		return ss.str();
	}

private:
	int m_sampling_rate; // sampling rate - num samples per second.
	std::vector<T> m_time_domain;
};

} /* namespace DSP */
} /* namespace SF */


#endif /* SIGNAL_H_ */
