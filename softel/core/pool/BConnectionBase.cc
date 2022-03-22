/*
 * BConnectionBase.cc
 *
 *  Created on: 23.03.2010
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "BConnectionBase.h"

using namespace std;

namespace SF {
namespace CORE {
namespace POOL {

BConnectionBase::BConnectionBase() {
	m_date_created.setNow();
}

BConnectionBase::BConnectionBase(int id)
	: m_id(id) {

	m_date_created.setNow();
}

BConnectionBase::~BConnectionBase() {

}

} /* namespace POOL */
} /* namespace CORE */
} /* namespace SF */
