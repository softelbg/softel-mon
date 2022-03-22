/*
 * SampleSet.h
 *
 *	Set of Samples - should represent a discrete function of time.
 *
 *  Created on: 10.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef SAMPLESET_H_
#define SAMPLESET_H_

#include "Sample.h"
#include <softel/db/CParamSet.h>
#include <softel/core/lock/CLockableObject.h>
#include <softel/core/lock/BoostSynchronized.h>
#include <softel/core/Ptr.h>
#include <softel/core/util/PtrVector.h>

namespace SF {
namespace MON {
namespace CORE {

template <class T>
class SampleSet : public SF::CORE::LOCK::CLockableObject {
public:
	SampleSet();
	virtual ~SampleSet();

	bool get(size_t pos, SF::CORE::Ptr<Sample<T> >& result);
	void add(SF::CORE::Ptr<Sample<T> >& sample);

	size_t size();
	void clear();

	SF::DB::CParamSet& getParamSet();

private:

	bool validatePos(size_t pos);

	SF::CORE::UTIL::PtrVector<Sample<T> > m_sample_set;
	//std::vector<SF::CORE::Ptr<Sample<T> > > m_sample_set;

	long m_period;
	SF::DB::CParamSet m_param_set;

};

} /* namespace CORE */
} /* namespace MON */
} /* namespace SF */

#endif /* SAMPLESET_H_ */
