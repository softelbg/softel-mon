/*
 * CSharedCounter.h
 *
 *  Created on: 21.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include <sys/shm.h>
#include <softel/core/inc/ErrorMsg.h>
#include <softel/core/error/CErrorCommon.h>
#include "CSharedCounter.h"

using namespace SF::CORE::ERROR;

namespace SF {
namespace CORE {
namespace UTIL {

CSharedCounter::CSharedCounter() {
	m_SharedMemID = shmget(IPC_PRIVATE, sizeof(struct SharedMem),
			(IPC_CREAT | 0600));
	if (m_SharedMemID == -1)
		throw CErrorCommon(FEFAILEDSHMGET);

	m_pSharedMem = ((volatile struct SharedMem *) shmat(m_SharedMemID, 0, 0));
	if ((long) m_pSharedMem == -1)
		throw CErrorCommon(FEFAILEDSHMAT);

	if (shmctl(m_SharedMemID, IPC_RMID, 0) == -1)
		throw CErrorCommon(FEFAILEDSHMCTL);

	m_pSharedMem->count = 0;
	m_pSharedMem->position = 0;
	m_pSharedMem->locked = false;
}
;

CSharedCounter::~CSharedCounter() {
}
;

void CSharedCounter::IncCounter() {
	m_pSharedMem->count++;
}
;

void CSharedCounter::DecCounter() {
	m_pSharedMem->count--;
}
;

int CSharedCounter::GetCounter() {
	return m_pSharedMem->count;
}

void CSharedCounter::resetCounter() {
	m_pSharedMem->count = 0;
}

void CSharedCounter::incPosition() {
	m_pSharedMem->position++;
}

void CSharedCounter::decPosition() {
	m_pSharedMem->position--;
}

int CSharedCounter::getPosition() {
	return m_pSharedMem->position;
}

void CSharedCounter::setLock(bool val) {
	m_pSharedMem->locked = val;
}

bool CSharedCounter::isLocked() {
	return m_pSharedMem->locked;
}

void CSharedCounter::incCounter(int pos, int step = 1) {

	if (isCounterIndexValid(pos)) {

		m_pSharedMem->arr_count[pos] += step;

	}
}

void CSharedCounter::decCounter(int pos, int step = 1) {

	if (isCounterIndexValid(pos)) {

		m_pSharedMem->arr_count[pos] -= step;

	}

}

int CSharedCounter::getCounter(int pos) {

	if (isCounterIndexValid(pos)) {

		return m_pSharedMem->arr_count[pos];

	} else {

		return 0;

	}

}

void CSharedCounter::resetCounter(int pos) {

	if (isCounterIndexValid(pos)) {

		m_pSharedMem->arr_count[pos] = 0;

	}
}

bool CSharedCounter::isCounterIndexValid(int pos) {

	if (pos < sizeof(m_pSharedMem->arr_count) / sizeof(int)) {

		return true;

	} else {

		return false;

	}

}

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */

