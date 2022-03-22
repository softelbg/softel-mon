/*
 * CommonAlert.h
 *
 *  Created on: 01.08.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef COMMONALERT_H_
#define COMMONALERT_H_

#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>


namespace SF {
namespace OSS {
namespace ALERT {

// Tables
#define TABLE_OSS_ALERT_CM_LIST			"oss_alert_cm_list"		// CM Alert List
#define TABLE_OSS_ALERT_BASE			"oss_alert_base"		// Base Alert
#define TABLE_OSS_EVENT_BASE			"oss_event_base"		// Base Event

#define TABLE_OSS_ALERT_CM				"oss_alert_cm"			// CM Alerts
#define TABLE_OSS_ALERT_IF				"oss_alert_if"			// IF Alerts


// Sync with sql tables...

//	Alerts
#define PARAM_ALERT_ID				"aid"
#define PARAM_ALERT_TYPE			"atype"
#define PARAM_ALERT_STATUS			"astatus"
#define PARAM_ALERT_COUNTER			"acounter"
#define PARAM_ALERT_DATE_INIT		"adateinit"
#define PARAM_ALERT_DATE			"adate"
#define PARAM_ALERT_SEVERITY		"aseverity"
#define PARAM_ALERT_MESSAGE			"amessage"


#define TYPE_ALERT_BASE				(1)
#define TYPE_ALERT_CMTS_IF			(2)
#define TYPE_ALERT_CMTS_CM			(3)
#define TYPE_ALERT_CM_DEDICATED		(4)
#define TYPE_ALERT_CMTS_IF_COUNTER	(5)

#define SEVERITY_ALERT_VHIGH		(0)
#define SEVERITY_ALERT_HIGH			(1)
#define SEVERITY_ALERT_MIDDLE		(2)
#define SEVERITY_ALERT_LOW			(3)



//	Events
#define PARAM_EVENT_ID				"eid"
#define PARAM_EVENT_TYPE			"etype"
#define PARAM_EVENT_STATUS			"estatus"
#define PARAM_EVENT_COUNTER			"ecounter"
#define PARAM_EVENT_DATE_INIT		"edateinit"
#define PARAM_EVENT_DATE			"edate"
#define PARAM_EVENT_MESSAGE			"emessage"

#define TYPE_EVENT_BASE				(1)
#define TYPE_EVENT_CMTS_IF			(2)


//	Thresholds
#define PARAM_THRES_VALUE			"thresh_val"
#define PARAM_THRES_CONDITION		"cond"
#define PARAM_THRES_SEVERITY		"severity"
#define PARAM_THRES_COLOR			"color"

#define CONDITION_LESS				"<"
#define CONDITION_MORE				">"


} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */

#endif /* COMMONALERT_H_ */
