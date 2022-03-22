/*
 * SnmpOids.h
 *
 *	SNMP OIDS
 *
 *  Created on: 01.12.2011
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef SnmpOids_H
#define SnmpOids_H

namespace SF {
namespace OSS {
namespace CMTS {

// 			Snmp Oids

//			Interface OIDs
#define SNMP_OID_IFNAME 			".1.3.6.1.2.1.31.1.1.1.1" 			// IF-MIB::ifName
#define SNMP_OID_IFDESC 			"1.3.6.1.2.1.2.2.1.2" 				// IF-MIB::ifDesc
#define SNMP_OID_IFTYPE 			".1.3.6.1.2.1.2.2.1.3"				// IF-MIB::ifType
#define SNMP_OID_IF_ALIAS 			".1.3.6.1.2.1.31.1.1.1.18"			// IF-MIB::ifAlias

#define SNMP_OID_IF_INOCT			"1.3.6.1.2.1.2.2.1.10"				// ifInOctets
#define SNMP_OID_IF_INUPKTS			"1.3.6.1.2.1.2.2.1.11"				// ifInUcastPkts
#define SNMP_OID_IF_OUTOCT			"1.3.6.1.2.1.2.2.1.16"				// ifOutOctets
#define SNMP_OID_IF_OUTUPKTS		"1.3.6.1.2.1.2.2.1.17"				// ifOutUcastPkts
#define SNMP_OID_IF_INOCT_HC		"1.3.6.1.2.1.31.1.1.1.6"			// ifHCInOctets
#define SNMP_OID_IF_OUTOCT_HC		"1.3.6.1.2.1.31.1.1.1.10"			// ifHCOutOctets

#define SNMP_OID_IF_CM_REGISTERED	"1.3.6.1.4.1.9.9.116.1.4.1.1.5"		// cdxIfUpChannelCmRegistered
#define SNMP_OID_IF_AVG_UTIL		"1.3.6.1.4.1.9.9.116.1.4.1.1.7"		// cdxIfUpChannelAvgUtil


//			CM OIDs
#define SNMP_OID_CM_LIST 			"1.3.6.1.2.1.10.127.1.3.3.1.2"		// SNMPv2-SMI::transmission.127.1.3.3.1.2
#define SNMP_OID_CM_IP				"1.3.6.1.2.1.10.127.1.3.3.1.3"		// SNMPv2-SMI::transmission.127.1.3.3.1.3
#define SNMP_OID_CM_STATUS			"1.3.6.1.2.1.10.127.1.3.3.1.9"		// SNMPv2-SMI::transmission.127.1.3.3.1.9

#define SNMP_OID_CM_CH_DWN			"1.3.6.1.2.1.10.127.1.3.3.1.4"		// SNMPv2-SMI::transmission.127.1.3.3.1.4
#define SNMP_OID_CM_CH_UP			"1.3.6.1.2.1.10.127.1.3.3.1.5"		// SNMPv2-SMI::transmission.127.1.3.3.1.5

// root oids - usualy on modem is .3 if(now using snmp bulk walk and get first element)
#define SNMP_OID_CM_SNR_DWN			"1.3.6.1.2.1.10.127.1.1.4.1.5"		// docsIfSigQSignalNoise
#define SNMP_OID_CM_RXP_DWN			"1.3.6.1.2.1.10.127.1.1.1.1.6"		// docsIfDownChannelPower
#define SNMP_OID_CM_TXP_UP			"1.3.6.1.2.1.10.127.1.2.2.1.3"		// docsIfCmStatusTxPower

#define SNMP_OID_CM_IF_IN			"1.3.6.1.2.1.2.2.1.10.3"			// ifInOctets.3
#define SNMP_OID_CM_IF_OUT			"1.3.6.1.2.1.2.2.1.16.4"			// ifOutOctets.4

#define SNMP_OID_CM_SYS_DESC		"1.3.6.1.2.1.1.1.0"					// SNMPv2-MIB::sysDescr.0

#define SNMP_OID_CM_DOCS3_DS_FREQ	"1.3.6.1.2.1.10.127.1.1.1.1.2"
#define SNMP_OID_CM_DOCS3_DS_PWR	"1.3.6.1.2.1.10.127.1.1.1.1.6"
#define SNMP_OID_CM_DOCS3_DS_SNR	"1.3.6.1.2.1.10.127.1.1.4.1.5"
#define SNMP_OID_CM_DOCS3_US_TXPWR	"1.3.6.1.4.1.4491.2.1.20.1.2.1.1"

//	 		Upstream signal quality
#define SNMP_OID_CMTS_UP_AVG_SNR 	"1.3.6.1.2.1.10.127.1.1.4.1.5"		// docsIfSigQSignalNoise
#define SNMP_OID_CMTS_UP_UNERR		"1.3.6.1.2.1.10.127.1.1.4.1.2"		// docsIfSigQUnerroreds
#define SNMP_OID_CMTS_UP_CORR		"1.3.6.1.2.1.10.127.1.1.4.1.3"		// docsIfSigQCorrecteds
#define SNMP_OID_CMTS_UP_UNCORR		"1.3.6.1.2.1.10.127.1.1.4.1.4"		// docsIfSigQUncorrectables
#define SNMP_OID_CMTS_UP_MR			"1.3.6.1.2.1.10.127.1.1.4.1.6"		// docsIfSigQMicroreflections
#define SNMP_OID_CMTS_UP_TxTO		"1.3.6.1.2.1.10.127.1.1.2.1.6"		// docsIfUpChannelTxTimingOffset


//			CPE Info (IP...)
#define SNMP_OID_CMTS_CPE_IP		"1.3.6.1.4.1.9.9.116.1.3.1.1.3"		// cdxCmCpeIpAddress
#define SNMP_OID_CMTS_MAC2SID		"1.3.6.1.4.1.9.9.116.1.3.1.1.6"		// cdxCmCpeCmStatusIndex




//		OID Keys
//		CMs
#define KEY_SNMP_OID_CM_CMTS_ID		"cmts_id"
#define KEY_SNMP_OID_CM_CMTS_IP		"cmts_ip"
#define KEY_SNMP_OID_CM_MAC_DEC		"mac_dec"
#define KEY_SNMP_OID_CM_MAC_HEX		"mac_hex"
#define KEY_SNMP_OID_CM_CH_DWN		"ifDwn"
#define KEY_SNMP_OID_CM_CH_UP		"ifUp"

//			IFs
#define KEY_SNMP_OID_IF_ID			"ifId"
#define KEY_SNMP_OID_IF_NAME		"ifName"
#define KEY_SNMP_OID_IF_DESC		"ifDesc"
#define KEY_SNMP_OID_IF_TYPE		"ifType"
#define KEY_SNMP_OID_IF_ALIAS		"ifAlias"


#define KEY_PARAM_OID_SNMP			"oid_snmp"
#define KEY_PARAM_OID_ID			"type_id"
#define KEY_PARAM_OID_LABEL			"oid_label"
#define KEY_PARAM_OID_DESC			"oid_desc"
#define KEY_PARAM_OID_DIM			"oid_dimension"
#define KEY_PARAM_OID_FN			"is_fn"
#define KEY_PARAM_COL_KEY			"col_key"
#define KEY_PARAM_OID_MULT			"oid_mult"
#define KEY_PARAM_OID_VAL_TYPE		"oid_value_type"

#define KEY_PARAM_FROM_CM			"from_cm"
#define KEY_PARAM_PRIO				"prio"
#define KEY_PARAM_CHANNEL			"chn"
#define KEY_PARAM_DOCSIS			"docsis"


#define KEY_PARAM_DIM_CW			"CW"
#define KEY_PARAM_DIM_DB			"dB"
#define KEY_PARAM_DIM_DBMV			"dBmV"
#define KEY_PARAM_DIM_DBC			"dBc"
#define KEY_PARAM_DIM_MBIT			"Mbit"

#define KEY_PARAM_EVENT_MESSAGE		"event_message"

//		Status
#define STATUS_CM_REGISTRATION_COMPLETE				6
#define STATUS_CM_REGISTRATION_COMPLETE_STR			"6"
#define STATUS_CM_OTHER_STR							"1"

#define NUM_COMPLEMENTARY_SNMP_OID_OFFSET			100000

#define THRESHOLD_PRIO_INVALID						100
#define THRESHOLD_PRIO_LOW							20
#define THRESHOLD_PRIO_NORMAL						10

#define CHANNEL_VAL_UP								"u"
#define CHANNEL_VAL_DOWN							"d"
#define CHANNEL_VAL_UNKNOWN							"x"

#define KEY_CHN_DOWNSTREAM							128
#define KEY_CHN_UPSTREAM							129
#define KEY_CHN_ETHERNET							6

#define KEY_COL_KEY_UP_SNR					"up_snr"
#define KEY_COL_KEY_DWN_SNR					"dwn_snr"
#define KEY_COL_KEY_NUM_ACTIVE_CM			"num_active_cm"
#define KEY_COL_KEY_IF_IN					"ifIn"
#define KEY_COL_KEY_IF_OUT					"ifOut"

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

#endif
