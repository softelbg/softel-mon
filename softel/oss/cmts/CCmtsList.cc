/**
 * List of CMTS
 * Retrieved from MySql DB
 *
 *	@author sgeorgiev
 *
 *		www.softel.bg
 *
 *		Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CCmtsList.h"

#include <softel/core/error/CErrorSql.h>
#include <softel/core/error/CErrorCommon.h>

using namespace std;
using namespace SF::DB;
using namespace SF::CORE::ERROR;
using namespace SF::CORE::NET;
using namespace SF::CORE::UTIL;
using namespace SF::CORE::SYSTEM;

namespace SF {
namespace OSS {
namespace CMTS {

CCmtsList::CCmtsList() {

	initCmtsList();

}

CCmtsList::~CCmtsList() {
	m_pDbConnPool->clearMySqlPool();
}

/**
 * Get CMTS List from DB
 */
void CCmtsList::getCmtsList(vector<CCmts*>& cmtsList) {

	CMySqlDB* pDbConn = m_pDbConnPool->getMySqlConn();

	getCmtsList(pDbConn, cmtsList);

	m_pDbConnPool->freeMySqlConn(pDbConn);
}

void CCmtsList::getCmtsList(CMySqlDB* pDbConn, vector<CCmts*>& cmtsList) {

	string cmtsId;
	string cmtsIp;
	string cmtsRComm;
	string cmRComm;
	string cmtsDesc;

	CDataSet dsCmList;
	CDataRow row;
	CCmtsToolsSql sqlTools;

	sqlTools.sqlGetActiveCmtsList(*pDbConn, dsCmList);

	cmtsList.clear();

	while (dsCmList.getNextRow(row)) {

		row.getColumn(KEY_COL_CMTS_ID, cmtsId);
		row.getColumn(KEY_COL_CMTS_IP, cmtsIp);
		row.getColumn(KEY_COL_CMTS_RCOMM, cmtsRComm);
		row.getColumn(KEY_COL_CM_RCOMM, cmRComm);
		row.getColumn(KEY_COL_CMTS_DESC, cmtsDesc);

		CCmts* currentCmts = new CCmts(cmtsId, cmtsIp, cmtsRComm, cmRComm,
				cmtsDesc, 2);

		cmtsList.push_back(currentCmts);

	}

	dsCmList.dumpInfo();

}

void CCmtsList::listCmts() {
	listCmts(m_processCounter);
}
/**
 * Make CMTS List. Fork every CMTS and launch needed cmts actions.
 */
void CCmtsList::listCmts(CSharedCounter& processCounter) {

	unsigned int i;

	vector<CCmts*> cmtsList;

	//LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ":PID:" << getpid() << endl;

	getCmtsList(cmtsList);

	CCmts* pCmts;

	for (i = 0; i < cmtsList.size(); i++) {

		pCmts = cmtsList[i];

		pCmts->setSharedCounter(&processCounter);
		pCmts->setCmtsPosition(i);
		pCmts->setAccountDb(m_pDbFactory);
		pCmts->setPeriodSleep(m_periodSleep);

		// Fork for every CMTS...
		int childPid = fork();
		//LOGD << "FORK:" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ":PID:" << getpid() << endl;
		if (childPid == 0) {

			processCounter.IncCounter();

			pCmts->dumpCmtsInfo();
		//	pCmts->startCmtsCollectingSingle();
			pCmts->startCmtsCollecting();
			
			break;

		} else {
			if (childPid < 0) {
				throw CErrorCommon(ERR_FORK);
			}
			//addCmts(pCmts);
		}
	}

}

/**
 * Initialize CMTS List.
 * Read ini file and set DbFactory with db account.
 * Read some other params...
 */
void CCmtsList::initCmtsList() {

	string intParam;

	m_periodSleep = 600;

	intParam = CCmtsFileIni::getInstance()->get("System", "PeriodSleep");
	if (intParam.size() > 0) {
		m_periodSleep = CStringUtil::toInt(intParam);
	}

	m_pDbFactory = new CSqlDbFactory(	CCmtsFileIni::getInstance()->get("MySQL", "Host"),
										CCmtsFileIni::getInstance()->get("MySQL", "User"),
										CCmtsFileIni::getInstance()->get("MySQL", "Pass"),
										CCmtsFileIni::getInstance()->get("MySQL", "Database")
									);

	m_pDbConnPool = m_pDbFactory->newConnPool();
	m_pDbConnPool->startMySqlPool(1);

}

/**
 * Launch CMTS processes and wait for them. Make proper schedule for each cmts.
 * TODO: Remove from here and use CScheduler.
 */
void CCmtsList::launchCmts() {

	int childStatus = 0;

	CTimeCounter tcMain;
	CTimeCounter tcChild;

	m_pidMainProcess = getpid();

	LOGD << "Main Process PID: " << m_pidMainProcess << endl;

	while (true) {

		LOGD << "pid:" << getpid() << "Proc Count: "
				<< m_processCounter.GetCounter() << " Elapsed "
				<< tcMain.getElapsed() << " s  " << tcMain.getCTimeNow()
				<< endl;

		tcChild.reset();

		m_processCounter.resetCounter();
		m_processCounter.resetCounter(0);

		m_processCounter.IncCounter();

		try {

			listCmts();
			//cmtsList.dumpCmtsList();

		} catch (CErrorSql& error) {
			LOG(LOG_LEVEL_ERROR, error.getMessage());
		} catch (CErrorCommon& error) {
			LOG(LOG_LEVEL_ERROR, error.getMessage());
		};

		m_processCounter.DecCounter();

		LOGD << "pid:" << getpid() << " Proc Count: "
				<< m_processCounter.GetCounter();
		LOGD << " Elapsed " << tcChild.getElapsed() << " s  "
				<< tcChild.getCTimeNow() << endl;

		if (m_pidMainProcess == getpid()) {

			pid_t childPid;

			do {

				childPid = wait(&childStatus);

				LOGD << "Child pid:" << childPid << " Ended with status:"
						<< childStatus;
				LOGD << " Elapsed " << tcChild.getElapsed() << " s  "
						<< tcChild.getCTimeNow() << endl;

				if (childStatus != 0) {
					m_processCounter.DecCounter();
				}

			} while (childPid > 0);

			childPid = wait(&childStatus);
			LOGD << "Final Child pid:" << childPid << " Ended with status:"
					<< childStatus;
			LOGD << " Elapsed " << tcMain.getElapsed() << " s  "
					<< tcMain.getCTimeNow() << endl;

			// Wait if there is remaining time for sleeping
			time_t elapsed = tcMain.getElapsed();
			int remainingSleep = m_periodSleep - elapsed;
	
			LOGD << "remainingSleep: " << remainingSleep << " m_periodSleep: " << " elapsed: " << elapsed << endl;

			if (remainingSleep > 0) {
				LOGD << "Main Process Sleeping for " << remainingSleep
						<< " s..." << endl;
				sleep(remainingSleep);
			}

			tcMain.reset();

		} else {
			break;
		}

	}
}

void CCmtsList::addCmts(CCmts* pCmts) {
	m_cmts_list.push_back(pCmts);
}

/**
 * For testing dump all cmts in the list
 */
void CCmtsList::dumpCmtsList() {

	for (unsigned int i = 0; i < m_cmts_list.size(); i++) {
		m_cmts_list[i]->dumpCmtsInfo();
	}

	LOGD << __FILE__ << ": End Pid: " << getpid() << endl;
}

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */
