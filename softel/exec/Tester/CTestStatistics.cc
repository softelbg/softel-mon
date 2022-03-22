/*
 * CTestStatistics.cc
 *
 *  Created on: 06.02.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CTestStatistics.h"
#include "CTestSqlTools.h"
#include <softel/db/CDataRow.h>
#include <softel/db/CDataSet.h>
#include <softel/core/error/CErrorSql.h>

using namespace std;
using namespace SF::DB;
using namespace SF::CORE::ERROR;

namespace SF {
namespace OSS {
namespace EXEC {

CTestStatistics::CTestStatistics() {
	// TODO Auto-generated constructor stub

}

CTestStatistics::~CTestStatistics() {
	// TODO Auto-generated destructor stub
}

/**
 * Make statistics for low performance (low UpSnr) for upstreams
 *
 */
void CTestStatistics::statPeriodLowSnrCmtsIfUp(const string& cmts_id) {

	cout << __FILE__ << ":" << __FUNCTION__ << " cmtsid: " << cmts_id << endl;

	CTestSqlTools sqlTools;

	CDataSet dsListIf;

	sqlTools.selectListCmtsIfUp(cmts_id, dsListIf);

	vector<string> listTables;
	listTables.push_back("201301");
	listTables.push_back("201302");

	for(int i = 0; i < dsListIf.size(); i++) {

		CDataSet dsFnDataIf;
		CDataRow rowIf;

		dsListIf.getRow(i, rowIf);

		cout << __FILE__ << ":" << __FUNCTION__ << " ifid: " << rowIf.getColumn("interface_id") << endl;

		try {

			sqlTools.selectFnDataCmtsIf(cmts_id, rowIf.getColumn("interface_id"), listTables, dsFnDataIf);

		} catch (CErrorSql& e) {
			cout << __FILE__ << ":" << __FUNCTION__ << " cmtsid: " << cmts_id;
			cout << " Error: " << e.getMessage() << endl;
			continue;
		}

		doStatPeriodCmtsIf(cmts_id, rowIf.getColumn("interface_id"), dsFnDataIf);

	}

}

void CTestStatistics::doStatPeriodCmtsIf(const string& cmts_id, const string& if_id,
		CDataSet ds) {

	vector<string> thrUpSnr;

	thrUpSnr.push_back("0");
	thrUpSnr.push_back("26");
	thrUpSnr.push_back("28");
	thrUpSnr.push_back("30");
	thrUpSnr.push_back("40");

	string col_key = "up_snr";

	for(size_t i = 0; i < thrUpSnr.size(); i++) {

		if (i > 0) {
			this->doStatPeriodCmtsIf(cmts_id, if_id, col_key, thrUpSnr[i - 1], thrUpSnr[i], ds);
		} else {
			this->doStatPeriodCmtsIf(cmts_id, if_id, col_key, "-1", "0", ds);
		}

	}

}

void CTestStatistics::run() {

	statPeriodLowSnrCmtsIfUp("171");

}

void CTestStatistics::doStatPeriodCmtsIf(const string& cmts_id,
		const string& if_id, const string& col_key, const string& thrDwn, const string& thrUp, CDataSet ds) {


	cout << __FILE__ << ":" << __FUNCTION__ << " cmtsid: " << cmts_id << " ifid: " << if_id;
	cout << " " << thrDwn << " < " << col_key << " <= " << thrUp << endl;

	double nThrDwn = atof(thrDwn.c_str());
	double nThrUp = atof(thrUp.c_str());

	double sumPeriod = 0;
	double sumCorrected = 0;
	double sumUnCorrected = 0;
	int countHits = 0;

	for(int i = 1; i < ds.size() - 1; i++) {

		CDataRow rowCurrent;

		ds.getRow(i, rowCurrent);

		string sParam = rowCurrent.getColumn(col_key);
		double nParam = atof(sParam.c_str());

		if (sParam.compare("0") != 0 && nParam == 0) {

			cout << __FILE__ << ":" << __FUNCTION__ << " cmtsid: " << cmts_id << " ifid: " << if_id;
			cout << " ERROR: " << sParam << " " << nParam << endl;

			continue;
		}

		double nDumpDateCurrent = atof(rowCurrent.getColumn("dumpdate").c_str());

		if (nParam > nThrDwn && nParam <= nThrUp) {

			CDataRow rowPrev;
			CDataRow rowNext;
			ds.getRow(i - 1, rowPrev);
			ds.getRow(i + 1, rowNext);

			//string sParamPrev = rowPrev.getColumn(col_key);
			//double nParamPrev = atof(sParamPrev.c_str());

			double nDumpDatePrev = atof(rowPrev.getColumn("dumpdate").c_str());
			//double nDumpDateNext = atof(rowNext.getColumn("dumpdate").c_str());

			countHits++;
			sumPeriod = sumPeriod + (nDumpDateCurrent - nDumpDatePrev);

			sumCorrected = sumCorrected + atof(rowPrev.getColumn("up_corrected").c_str());
			sumUnCorrected = sumUnCorrected + atof(rowPrev.getColumn("up_uncorrected").c_str());

		}

	}

	cout << __FILE__ << ":" << __FUNCTION__ << " cmtsid: " << cmts_id << " ifid: " << if_id;
	cout << " " << thrDwn << " < " << col_key << " <= " << thrUp;
	cout << " avgCorrected: " << (sumCorrected / countHits) << " % ";
	cout << " avgUnCorrected: " << (sumUnCorrected / countHits) << " % ";
	cout << " Period: " << sumPeriod << " seconds" << endl;

	cout << __FILE__ << ":" << __FUNCTION__ << " cmtsid: " << cmts_id << " ifid: " << if_id;
	cout << " " << thrDwn << " < " << col_key << " <= " << thrUp;
	cout << " avgCorrected: " << (sumCorrected / countHits) << " % ";
	cout << " avgUnCorrected: " << (sumUnCorrected / countHits) << " % ";
	cout << " Period: " << (sumPeriod / 3600) << " hours" << endl;

	cout << __FILE__ << ":" << __FUNCTION__ << " cmtsid: " << cmts_id << " ifid: " << if_id;
	cout << " " << thrDwn << " < " << col_key << " <= " << thrUp;
	cout << " avgCorrected: " << (sumCorrected / countHits) << " % ";
	cout << " avgUnCorrected: " << (sumUnCorrected / countHits) << " % ";
	cout << " Period: " << (sumPeriod / (3600*24)) << " days" << endl;

}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */



