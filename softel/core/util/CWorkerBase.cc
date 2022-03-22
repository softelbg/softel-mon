/*
 * CWorkerBase.cc
 *
 *  Created on: 09.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CWorkerBase.h"

#include <softel/core/system/CSystemTools.h>

using namespace SF::CORE::SYSTEM;

namespace SF {
namespace CORE {
namespace UTIL {

CWorkerBase::CWorkerBase() {

	this->setFree();

}

CWorkerBase::~CWorkerBase() {
	// TODO Auto-generated destructor stub
}

void CWorkerBase::setFree() {
	this->m_free = true;
	this->m_pJob = 0;
}

void CWorkerBase::setBusy() {
	this->m_free = false;
}

bool CWorkerBase::isFree() {
	return this->m_free;
}

/**
 * Set a new job if the worker is free and go to busy mode.
 *
 */
bool CWorkerBase::setJob(CWorkerJob* pJob) {

	if (this->isFree()) {

		this->m_pJob = pJob;
		this->setBusy();

	} else {
		return false;
	}

	return true;
}

/**
 * Worker Run implementation. Should be a non-exitable thread.
 * Exits only when Worker is Stopped.
 *
 */
int CWorkerBase::run() {

	while(this->shouldStop() == false) {

		if (this->isFree() == false) {
			this->doWorkerJob();
		}

		this->setFree();

		CSystemTools::doUSleep(100000); // TODO: should decide the actual sleep...
	}

	return 0;
}

/**
 * Run the Job.
 *
 */
void CWorkerBase::doWorkerJob() {
	if (this->m_pJob) {

		this->m_pJob->run();

		this->m_pJob->setCompleted();

		if (this->m_pJob->isDeletable()) {
			delete this->m_pJob;
		}
	}
}

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */




