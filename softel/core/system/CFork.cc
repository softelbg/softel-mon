/*
 * CFork.cc
 *
 *  Created on: 20.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CFork.h"

using namespace std;
using namespace SF::CORE::ERROR;

namespace SF {
namespace CORE {
namespace SYSTEM {

#define ERR_FORK	"Error Fork"

CFork::CFork() {

	pidChild = -1;

	pidSelf = -1;

}

/**
 * Do initialization Fork - Parent process should exit.
 * Only the Child should remain.
 * Used mainly in the beginning to avoid use of nohup...
 */
int CFork::doInitialFork() {

	doFork();

	if (isParent()) {
		CSystemTools::doExit(0);
	}

	return getChildPid();
}

/**
 * Do the Fork. Save returned child pid and get self pids.
 *
 */
int CFork::doFork() {

	pidChild = CSystemTools::doFork();

	pidSelf = CSystemTools::doGetPid();

	if (pidChild < 0) {
		throw CErrorCommon(ERR_FORK);
	}

	return pidChild;
}

/**
 * Check is Parent - should pidChild > 0
 */
bool CFork::isParent() {
	return (getChildPid() > 0);
}

/**
 * Check is Child - should pidChild = 0
 */
bool CFork::isChild() {
	return (getChildPid() == 0);
}

int CFork::getChildPid() {
	return pidChild;
}

int CFork::getSelfPid() {
	return pidSelf;
}

} /* namespace SYSTEM */
} /* namespace CORE */
} /* namespace SF */



