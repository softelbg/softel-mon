/*
 * ScheduledItem.cc
 *
 *  Created on: 30.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "ScheduledItem.h"

using namespace std;

namespace SF {
namespace CORE {
namespace POOL {

ScheduledItem::ScheduledItem(std::string id, long p) :
	m_id(id), m_period(p) {

}

ScheduledItem::ScheduledItem() :
	m_id("ScheduledItem"), m_period(600) {

}

ScheduledItem::~ScheduledItem() {

}

} /* namespace POOL */
} /* namespace CORE */
} /* namespace SF */
