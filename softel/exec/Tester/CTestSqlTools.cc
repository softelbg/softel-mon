/*
 * CTestSqlTools.cc
 *
 *  Created on: 06.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CTestSqlTools.h"
#include <softel/core/error/CErrorSql.h>

using namespace std;
using namespace SF::DB;
using namespace SF::CORE::ERROR;


namespace SF {
namespace OSS {
namespace EXEC {

CTestSqlTools::CTestSqlTools() {
	// TODO Auto-generated constructor stub

}

CTestSqlTools::CTestSqlTools(int numconn)
	: CSqlToolsBase(numconn) {

}

CTestSqlTools::~CTestSqlTools() {
	// TODO Auto-generated destructor stub
}

/**
 * Delete Duplicate CMs from oss_cm_list.
 * Usually when a modem has changed its CMTS it will appear more than once in the list.
 * Delete all of them and the next collection cycle will restore the newest one.
 */
void CTestSqlTools::deleteDuplicateListCm() {

	stringstream ssSql;

	CDataSet ds;
	CDataRow row;

	ssSql.clear();

	ssSql << "SELECT cm_mac from oss_cm_list group by cm_mac having count(cm_mac) > 1";

	this->sqlSelect(ssSql, ds);

	cout << "Duplicate CM count: " << ds.size() << endl;

	for(int i = 0; i < ds.size(); i++) {

		stringstream sqlDelete;

		ds.getRow(i, row);

		string cmMac = row.getColumn("cm_mac");

		cout << "PROCESS " << i << " " << cmMac << endl;

		sqlDelete << "DELETE FROM oss_cm_list WHERE cm_mac = " << cmMac;

		cout << "SQL: " << sqlDelete.str() << endl;

		this->sqlExecute(sqlDelete);
	}



}

/**
 * Search for Wrong Params and set it Zero.
 * Usually these are error functions after db restart
 * causing wrong diff calculation.
 */
void CTestSqlTools::zeroWrongParams(const string& tablePrefix, const string& tableDate) {

	stringstream ssSql;

	CDataSet ds;
	CDataRow row;

	map<string, int> listWrongParams;

	this->doWrongParamSet(listWrongParams);

	ssSql << "SELECT * FROM oss_cmts_list ";
	this->sqlSelect(ssSql, ds);

	for (int i = 0; i < ds.size(); i++) {

		ds.getRow(i, row);

		string tableName = tablePrefix + tableDate + "_" + row.getColumn("cmts_id");

		map<string, int>::iterator it;

		for(it = listWrongParams.begin(); it != listWrongParams.end(); it++) {

			stringstream ssUpdate;

			ssUpdate << "UPDATE " << tableName;
			ssUpdate << " SET " << it->first << " = 0 ";
			ssUpdate << " WHERE " << it->first << " > " << it->second;

			cout << "SQL: " << ssUpdate.str() << endl;

			this->sqlExecute(ssUpdate);
		}

	}

}

/**
 * Drop all memory tables.
 * Needed whem mysql server restarted.
 * TODO: Make it happen automatically or better managed...
 */
void CTestSqlTools::dropMemoryTables() {

	stringstream ssSql;

	CDataSet ds;
	CDataRow row;

	ssSql << "SELECT table_name ";
	ssSql << " FROM INFORMATION_SCHEMA.TABLES ";
	ssSql << " WHERE engine = \'MEMORY\' AND table_name like \'oss\%\'";

	this->sqlSelect(ssSql, ds);

	for(int i = 0; i < ds.size(); i++) {

		ds.getRow(i, row);

		stringstream ssDrop;

		ssDrop << "DROP TABLE " << row.getColumn("table_name");

		cout << "SQL: " << ssDrop.str() << endl;

		this->sqlExecute(ssDrop);

	}


}

/**
 * Alter Tables for function params.
 * Needed table prefix and data(if date is empty alter today/latest tables).
 * Column Part should contain name of column, type of param and some options if needed.
 */
void CTestSqlTools::alterTablesAddColumn(const string& tablePrefix, const string& tableDate, const string& colPart) {

	stringstream ssSql;

	CDataSet ds;
	CDataRow row;

	ssSql << "SELECT * FROM oss_cmts_list ";
	this->sqlSelect(ssSql, ds);

	for (int i = 0; i < ds.size(); i++) {

		ds.getRow(i, row);

		string tableName = tablePrefix + tableDate + "_" + row.getColumn("cmts_id");

		stringstream ssAlter;
		ssAlter << "ALTER TABLE " << tableName << " ADD " << colPart;

		cout << "SQL: " << ssAlter.str() << endl;
		this->sqlExecute(ssAlter);

	}

	if (tableDate.size() == 0) {
		stringstream ssAlterToday;
		ssAlterToday << "ALTER TABLE " << tablePrefix << "today" << " ADD " << colPart;
		cout << "SQL: " << ssAlterToday.str() << endl;
		this->sqlExecute(ssAlterToday);

		stringstream ssAlterLatest;
		ssAlterLatest << "ALTER TABLE " << tablePrefix << "latest" << " ADD " << colPart;
		cout << "SQL: " << ssAlterLatest.str() << endl;
		this->sqlExecute(ssAlterLatest);
	}

}

/**
 * Select List of Upstreams for a CMTS
 */
void CTestSqlTools::selectListCmtsIfUp(const string& cmts_id, CDataSet& ds) {
	this->selectListCmtsIf(cmts_id, "129", ds);
}
/**
 * Select List of IFs for a CMTS
 */
void CTestSqlTools::selectListCmtsIf(const string& cmts_id, const string& if_type, CDataSet& ds) {

	stringstream ssSql;

	ssSql << "SELECT * FROM oss_cmts_interface ";
	ssSql << " WHERE cmts_id = " << cmts_id;
	ssSql << " AND interface_type = " << if_type;
	ssSql << " AND count_valid < 70";

	this->sqlSelect(ssSql, ds);

}

/**
 * Select all Function Data records for a cmts if with a givven list of tables to make UNION.
 *
 */
void CTestSqlTools::selectFnDataCmtsIf(const string& cmts_id,
		const string& if_id, const vector<string>& listTables, CDataSet& ds) {

	stringstream ssSql;

	for(size_t i = 0; i < listTables.size(); i++) {
		ssSql << "SELECT * FROM " << "oss_if_" << listTables[i] << "_" << cmts_id;
		ssSql << " WHERE cmts_id = " << cmts_id;
		ssSql << " AND if_id = " << if_id;

		if (i < listTables.size() - 1) {
			ssSql << " UNION ";
		}
	}

	ssSql << " ORDER BY dumpdate";

	this->sqlSelect(ssSql, ds);
}

/**
 * Drop Big(CM/IF function) Tables.
 * Sweep all by cmts_id.
 */
void CTestSqlTools::dropBigTables(const string& tablePrefix,
		const string& tableDate) {

	stringstream ssSql;

	CDataSet ds;
	CDataRow row;

	ssSql << "SELECT * FROM oss_cmts_list ";
	this->sqlSelect(ssSql, ds);

	for (int i = 0; i < ds.size(); i++) {

		ds.getRow(i, row);

		string tableName = tablePrefix + tableDate + "_" + row.getColumn("cmts_id");

		stringstream ss;
		ss << "DROP TABLE " << tableName;

		cout << "SQL: " << ss.str() << endl;

		try {
			this->sqlExecute(ss);
		} catch (CErrorSql& e) {
			cout << "SqlError: " << e.getSql() << endl << "Msg: " << e.getMessage() << endl;
		}

	}

}

void CTestSqlTools::doWrongParamSet(map<string, int>& params) {

	params.clear();

	params["dwn_corrected"] = 100;
	params["dwn_uncorrected"] = 100;
	params["up_corrected"] = 100;
	params["up_uncorrected"] = 100;

}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */



