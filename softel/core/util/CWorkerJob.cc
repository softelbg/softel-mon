/*
 * CWorkerJob.cc
 *
 *  Created on: 10.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CWorkerJob.h"

namespace SF {
namespace CORE {
namespace UTIL {

CWorkerJob::CWorkerJob() {

	this->m_completed = false;

	this->setShouldDelete(false);
}

CWorkerJob::~CWorkerJob() {
	// TODO Auto-generated destructor stub
}

bool CWorkerJob::isCompleted() {
	return this->m_completed;
}

void CWorkerJob::setCompleted() {
	this->m_completed = true;
}

bool CWorkerJob::isDeletable() {
	return this->m_should_delete;
}

void CWorkerJob::setShouldDelete(bool shouldDelete) {
	m_should_delete = shouldDelete;
}

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */

