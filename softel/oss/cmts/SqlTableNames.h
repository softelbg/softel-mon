/*
 * SqlTableNames.h
 *
 *	SQL Table names, column keys...
 *
 *  Created on: 01.12.2011
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef SqlTableNames_H
#define SqlTableNames_H


namespace SF {
namespace OSS {
namespace CMTS {

// Some Common stuff
// TODO: Move to common .h
#define	FILE_CMTSMON_INIT		"/etc/CMTSMon.ini"
#define	FILE_CMTSMON_LOG		"/var/log/CMTSMon.log"



// SQL Table Names
#define TABLE_OSS_CMTS_LIST 			"oss_cmts_list"			// List with CMTS
#define TABLE_OSS_CMTS_INTERFACE 		"oss_cmts_interface"	// CMTS Interfaces
#define TABLE_OSS_CMTS_OID_SETS			"oss_cmts_oid_sets"		// CMTS OID Set
#define TABLE_OSS_CPE_OID_TYPES			"oss_cpe_oid_types"		// All defined OIDs
#define TABLE_OSS_CM_INFO				"oss_cm_info"			// CMs Info (client information and so on...)
#define TABLE_OSS_CM_LIST				"oss_cm_list"			// List of CMs
#define TABLE_OSS_CM_LIST_DOCS3			"oss_cm_list_docs3"		// List of Docsis 3 CMs (multiple if rows per CM)
#define TABLE_OSS_CM_LATEST				"oss_cm_latest"			// Latest OID values for CMs
#define TABLE_OSS_CM_ERRORS				"oss_cm_errors"			// Last Errors counters
#define TABLE_OSS_CM_CPE_LIST			"oss_cm_cpe_list"		// CM CPE List
#define TABLE_OSS_IF_LATEST				"oss_if_latest"			// Latest OID values for CMTS IFs
#define TABLE_OSS_IF_ERRORS				"oss_if_errors"			// Last Errors counters
#define TABLE_OSS_STORE					"oss_key_value_store"	// Common usage key->value store

// Table Name Prefixes
#define TABLE_NAME_PREFIX_OSS_CM 		"oss_cm_";			// Prefix for CM OID values tables
#define TABLE_NAME_PREFIX_OSS_CM_DOCS3	"oss_cm_docs3_";	// Prefix for CM Docsis 3 OID values tables
#define TABLE_NAME_PREFIX_OSS_CMTS_IF	"oss_if_";			// Prefix for CMTS IFs OID values tables

#define TABLE_NAME_POSTFIX_TODAY		"today"			// Memory table with functional params for the last day
#define KEY_POSTFIX_MIN_DATE			"_min_date"

//	 FROM CM Values (from CMTS CM, direct from CM, from CMTS for CMTS Up/Dwn)
#define VAL_FROM_CMTS_CM			"0"		// from CMTS for CM
#define VAL_FROM_CM					"1" 	// direct from CM
#define VAL_FROM_CMTS_IF			"2" 	// from CMTS for CMTS Up/Dwn iFs
#define VAL_FROM_NET_ELEM			"100"	// From Base Network Element

#define VAL_IS_NOT_FN				"0"		// Oid is Not a Function type - should update only Latest Table
#define VAL_IS_FN					"1"		// Oid is a Function type (should be added to additive table)


//		Table column keys
#define KEY_COL_CMTS_ID				"cmts_id"
#define KEY_COL_CMTS_IP				"cmts_ip"
#define KEY_COL_CMTS_RCOMM			"cmts_rcommunity"
#define KEY_COL_CM_RCOMM			"cm_rcommunity"
#define KEY_COL_CMTS_DESC			"cmts_desc"
#define KEY_COL_CMTS_VER			"cmts_ver"
#define KEY_COL_CMTS_PERIOD_SLEEP	"cmts_period_sleep"

#define KEY_COL_CM_MAC				"cm_mac"
#define KEY_COL_CM_IP				"cm_ip"
#define KEY_COL_CM_STATUS			"cm_status"
#define KEY_COL_CM_SID				"cm_sid"

#define KEY_COL_IF_ID				"if_id"
#define KEY_COL_IF_DWN				"if_dwn"
#define KEY_COL_IF_UP				"if_up"
#define KEY_COL_IF_TYPE				"interface_type"

#define KEY_COL_DWN_TXPWR			"dwn_txpwr"
#define KEY_COL_UP_MODULATION		"up_modulation"

#define STORE_KEY_TODAY_MIN_DATE	"today_min_date"


} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

#endif
