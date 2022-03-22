/*
 * CSchedulerCmAlert.cc
 *
 *  Created on: 21.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CSchedulerCmAlert.h"

namespace SF {
namespace OSS {
namespace EXEC {

CSchedulerCmAlert::CSchedulerCmAlert() {

	setPeriodSleep(60);

	//initCmList();

}

CSchedulerCmAlert::~CSchedulerCmAlert() {

}

/**
 * Initialize CM Alert List.
 */
void CSchedulerCmAlert::initCmList() {

	m_cmList.initCmList();

}

/**
 * Implementation of run method which is scheduled.
 * Should use CM Alert List and monitor the CMs.
 * Produce proper Alerts based on monitored CMs.
 */
void CSchedulerCmAlert::run() {

	initCmList();

	m_cmList.monitorCmList();

}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

