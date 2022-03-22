/*
 * TableBase.cc
 *
 *  Created on: May 3, 2014
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2014 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "TableBase.h"

namespace SF {
namespace DB {

TableBase::TableBase(const string& nameTable) :
	m_table_name(nameTable) {

}

TableBase::~TableBase() {
	// TODO Auto-generated destructor stub
}

} /* namespace DB */
} /* namespace SF */
