/*
 * CThreadPoolBase.cc
 *
 *  Created on: 08.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CThreadPoolBase.h"

using namespace std;

namespace SF {
namespace CORE {
namespace UTIL {

CThreadPoolBase::CThreadPoolBase() {
	// TODO Auto-generated constructor stub

	CThreadPoolBase(10);
}

CThreadPoolBase::CThreadPoolBase(int size) {

	this->initialize(size);

}

CThreadPoolBase::~CThreadPoolBase() {
	// TODO Auto-generated destructor stub
}

int CThreadPoolBase::size() {
	return this->m_pool.size();
}

void CThreadPoolBase::clear() {
}

void CThreadPoolBase::initialize(int size) {

	for (int i = 0; i < size; i++) {

		this->add();

	}

}

CWorkerBase* CThreadPoolBase::get() {

	// TODO implemented easiest algo, make it better...
	for (int i = 0; i < this->size(); i++) {
		if (this->m_pool[i]->isFree()) {
			return this->m_pool[i];
		}
	}

	//this->add();
	//return this->m_pool[this->size() - 1];

	return NULL;
}

void CThreadPoolBase::add() {

	CWorkerBase* pWorker = new CWorkerBase();

	pWorker->start(1048576, true);

	this->m_pool.push_back(pWorker);
}

/**
 * Add a new job to the job queue.
 *
 */
void CThreadPoolBase::addJob(CWorkerJob* job) {
	this->m_job_queue.push_back(job);
}

/**
 * Process the Job Queue. Get next job and pass it to a free worker.
 * Should be called frequently.
 */
void CThreadPoolBase::processQueue() {

	while (true) {

		CWorkerBase* worker = this->get();

		if (worker) {

			if (this->m_job_queue.size() > 0) {

				vector<CWorkerJob*>::iterator it = this->m_job_queue.begin();
				CWorkerJob* job = this->m_job_queue.front();

				worker->setJob(job);

				this->m_job_queue.erase(it);
			} else {
				break;
			}

		} else {
			break;
		}
	}

}

int CThreadPoolBase::getJobQueueSize() {
	return this->m_job_queue.size();
}

void CThreadPoolBase::free(int pos) {
}

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */

