/*
 * CSqlNmsTools.cc
 *
 *  Created on: 03.03.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CSqlNmsTools.h"

using namespace std;
using namespace SF::DB;

namespace SF {
namespace OSS {
namespace NMS {

CSqlNmsTools::CSqlNmsTools() {
	// TODO Auto-generated constructor stub

}

CSqlNmsTools::~CSqlNmsTools() {
	// TODO Auto-generated destructor stub
}

/**
 * Select List of NetElems filtered
 */
void CSqlNmsTools::selectListNetElem(const string& filter, CDataSet& ds) {

	stringstream ssSql;

	ssSql << "SELECT * FROM oss_net_elem_list ";

	if (filter.size()) {
		ssSql << " WHERE " << filter << " ";
	}

	//cout << __FILE__ << ":" << __FUNCTION__ << " pid:" << CSystemTools::doGetPid();
	//cout << " SQL: " << ssSql.str() << endl;

	sqlSelect(ssSql, ds);
}

/**
 * Select Full List of UPS NetElems.
 */
void CSqlNmsTools::selectListNetElemUPS(CDataSet& ds) {
	this->selectListNetElem("type_id = 10", ds);
}

/**
 * Select SnmpOid List filtered by from_cm
 */
void CSqlNmsTools::selectListSnmpOid(const string& from, CDataSet& ds) {

	stringstream ssSql;

	ssSql << "SELECT * FROM oss_cpe_oid_types WHERE from_cm = " << from;

	sqlSelect(ssSql, ds);
}

void CSqlNmsTools::selectListSnmpOidNetElemBase(CDataSet& ds) {
	this->selectListSnmpOid("100", ds);
}

void CSqlNmsTools::selectListSnmpOidNetElemUps(CDataSet& ds) {
	this->selectListSnmpOid("101", ds);
}

} /* namespace NMS */
} /* namespace OSS */
} /* namespace SF */




