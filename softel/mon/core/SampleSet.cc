/*
 * SampleSet.cc
 *
 *  Created on: 10.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "SampleSet.h"

using namespace SF::CORE;

namespace SF {
namespace MON {
namespace CORE {

template<class T>
SF::MON::CORE::SampleSet<T>::SampleSet() :
	m_period(0) {
}

template<class T>
SF::MON::CORE::SampleSet<T>::~SampleSet() {
}

template<class T>
bool SF::MON::CORE::SampleSet<T>::validatePos(size_t pos) {
	return this->m_sample_set.validatePos(pos);
}

template<class T>
bool SF::MON::CORE::SampleSet<T>::get(size_t pos, Ptr<Sample<T> >& result) {

	if (this->validatePos(pos)) {
		result = this->m_sample_set[pos];
		return true;
	}

	return false;
}

template<class T>
void SF::MON::CORE::SampleSet<T>::add(Ptr<Sample<T> >& sample) {
	this->m_sample_set.push_back(sample);
}

template<class T>
size_t SF::MON::CORE::SampleSet<T>::size() {
	return this->m_sample_set.size();
}

template<class T>
void SF::MON::CORE::SampleSet<T>::clear() {

	for(int i = 0; i < size(); i++) {
		this->m_sample_set[i].reset();
	}

	this->m_sample_set.clear();
	this->m_param_set.clear();
}

template<class T>
SF::DB::CParamSet& SF::MON::CORE::SampleSet<T>::getParamSet() {
	return this->m_param_set;
}

} /* namespace CORE */
} /* namespace MON */
} /* namespace SF */
