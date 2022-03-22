/* **********************************************************
 * Copyright 2003 VMware, Inc.  All rights reserved. -- VMware Confidential
 * **********************************************************/

/*
 * VpxdSnmp.cpp --
 *
 *    Provides snmp operations.
 *
 */

#include "vpxservicesHeaders.h"

#include "vpxUtil.h"

#include "vpxdSnmp.h"

using namespace Vmacore;
using namespace Vmacore::System;

#define  SNMP_CONFIG_TT_PER "snmp.testTrap.periodic.period"
#define  SNMP_CONFIG_CS_EN "snmp.coldStartTrap.enable"
#define  SNMP_CONFIG_TT_EN "snmp.testTrap.periodic.enable"


/*
 * Static member variables.
 */

bool VpxdSNMP::_sSnmpColdStartTrapEnable(true);
bool VpxdSNMP::_sSnmpPeriodicTestTrapEnable(false);
int  VpxdSNMP::_sSnmpTestTrapInterval(300);

Ref<VpxdSNMP::SnmpSystemMo> VpxdSNMP::SnmpSystemMo::_instance = NULL;

/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::PackageBuilder::PackageBuilder --
 *
 *    Constructor.
 *
 *----------------------------------------------------------------------
 */

VpxdSNMP::PackageBuilder::PackageBuilder()
   : m_available(sizeof(m_buffer))
{
}


/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::PackageBuilder::PushOID --
 *
 *    Push the OID. OID is string seperated by dot.
 *
 *    Package is build as
 *       [type] [length] [node1] [ node2] ...
 *
 * Result:
 *    True on success, false on error.
 *
 *----------------------------------------------------------------------
 */

bool
VpxdSNMP::PackageBuilder::PushOID(int type, 
                                  const string& oid)
{
   vector<int> nodes;
   if (!SplitDotString(oid, nodes)) {
      return false;
   }

   int length = GetSize();

   if (nodes.size() == 0) {
      // Make OID have two with value of zero.
      if (!PushByte(0) || !PushByte(0)) {
         return false;
      }
   } else if (nodes.size() == 1) {
      if (!PushByte(nodes[0])) {
         return false;
      }
   } else {
      for (int i = nodes.size() - 1; i >= 2; i--) {
         int node = nodes[i];
         if (!PushByte((unsigned char)(node & 0x7F))) {
            return false;
         }
         node >>= 7;

         while (node > 0) {
            if (!PushByte((unsigned char)(node & 0x7F) | 0x80)) {
               return false;
            }
            node >>= 7;
         }
      }

      if (nodes[1] > 40) {
         return false;
      }

      // First two nodes are combined into one octet.
      unsigned char ch = (nodes[0] * 40) + nodes[1];
      if (!PushByte((unsigned char)ch)) {
         return false;
      }
   }

   length = GetSize() - length;
   return PushHeader(type, length);
}


/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::PackageBuilder::PushIP --
 *
 *    Push an IP address.
 *
 *    Package is built as
 *       [type] [length] [node1] [node2] [node3] [node4]
 *
 * Results:
 *    True on success, false on error.
 *
 *----------------------------------------------------------------------
 */

bool
VpxdSNMP::PackageBuilder::PushIP(int type,
                                 const string& ip)
{
   struct hostent *h = gethostbyname(ip.c_str());
   if (h == NULL) {
      return false;
   }
   struct in_addr addr = *(struct in_addr *)h->h_addr;

   // Push the 4 byte IP address.
   int length = GetSize();
   if (!PushBlock((const char*)&addr, sizeof(addr))) {
      return false;
   }

   length = GetSize() - length;
   return PushHeader(type, length);
}


/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::PackageBuilder::PushString --
 *
 *    Push a string.
 *
 *    Package is built as
 *       [type] [length] [char1] [char2] ...
 *
 * Results:
 *    True on success, false on error.
 *
 *----------------------------------------------------------------------
 */

bool
VpxdSNMP::PackageBuilder::PushString(int type,
                                     const string& str)
{
   if (!PushBlock(str.c_str(), str.length())) {
      return false;
   }

   return PushHeader(type, str.length());
}

/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::PackageBuilder::PushDateTime --
 *
 *    Push a DateTime object as SNMP DateAndTime TC construct.
 *
 *    Package is built as
 *       [type] [length] [octets as chars]* ...
 *
 *    where the SNMP DateAndTime specification defines octets as follows:
 *
 *    field  octets  contents                  range
 *    -----  ------  --------                  -----
 *    1      1-2     year*                     0..65536
 *    2       3      month                     1..12
 *    3       4      day                       1..31
 *    4       5      hour                      0..23
 *    5       6      minutes                   0..59
 *    6       7      seconds                   0..60
 *                   (use 60 for leap-second)
 *    7       8      deci-seconds              0..9
 *    8       9      direction from UTC        '+' / '-'
 *    9      10      hours from UTC*           0..13
 *   10      11      minutes from UTC          0..59
 *
 *    Notes:
 *     - the value of year is in network-byte order
 *     - daylight saving time in New Zealand is +13
 *
 *    For example, Tuesday May 26, 1992 at 1:30:15 PM EDT would be
 *    displayed as:
 *
 *                   1992-5-26,13:30:15.0,-4:0
 *
 * Results:
 *    True on success, false on error.
 *
 *----------------------------------------------------------------------
 */
bool
VpxdSNMP::PackageBuilder::PushDateTime(int type,
                                       const DateTime& dateTime)
{
   unsigned char dateTimeOctets[SNMP_DATETIME_LEN];

   // Do year
   // The below code is equivalent to:
   // unsigned short *dateTimeYr = (unsigned short *) &dateTimeOctets[0];
   // *dateTimeYr = (unsigned short)dateTime.GetYear();; //truncate to 0...65536(2^16 as required)
   int val = dateTime.GetYear();
   dateTimeOctets[0] = (unsigned char) val;
   dateTimeOctets[1] = (unsigned char)(val >> 8);

   // Do month ,day
   dateTimeOctets[2] = (unsigned char)(dateTime.GetMonth());
   dateTimeOctets[3] = (unsigned char)(dateTime.GetDay());

   // Do hour,min,sec, deci-seconds
   dateTimeOctets[4] = (unsigned char)(dateTime.GetHour());
   dateTimeOctets[5] = (unsigned char)(dateTime.GetMinute());
   dateTimeOctets[6] = (unsigned char)(dateTime.GetSecond());
   dateTimeOctets[7] = (unsigned char)(dateTime.GetMillisecond() / 10);

   // Do UTC time bias (from + or - in minutes)
   int bias = dateTime.GetBias();
   if (bias >= 0) {
      dateTimeOctets[8] = (unsigned char)'+';
   } else {
      dateTimeOctets[8] = (unsigned char)'-';
      bias *= -1; // make value positive
   }
   dateTimeOctets[9] = (unsigned char) (bias / 60);
   dateTimeOctets[10] = (unsigned char) (bias % 60);

   if (!PushUnsignedBlock(dateTimeOctets, SNMP_DATETIME_LEN)) {
      return false;
   }
   return PushHeader(type, SNMP_DATETIME_LEN);
}


/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::PackageBuilder::PushUnLong --
 *
 *    Push an unsigned long integer.
 *
 *    Package is built as
 *       [type] [length] [unsigned long integer]
 *
 * Results:
 *    True on success, false on error.
 *
 *----------------------------------------------------------------------
 */

bool
VpxdSNMP::PackageBuilder::PushUnLong(int type,
                                     unsigned long value)
{
   int length = GetSize();

   if (!PushByte((unsigned char)value)) {
      return false;
   }
   value >>= 8;

   while (value != 0) {
      if (!PushByte((unsigned char)value)) {
         return false;
      }
      value >>= 8;
   }

   unsigned char ch = GetBuffer()[0];
   if ((ch & 0x80) != 0) {
      if (!PushByte((unsigned char)0)) {
         return false;
      }
   }

   length = GetSize() - length;
   return PushHeader(type, length);
}


/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::PackageBuilder::PushLong --
 *
 *    Push a signed long integer.
 *
 *    Package is built as
 *       [type] [length] [signed integer]
 *
 * Results:
 *    True on success, false on error.
 *
 *----------------------------------------------------------------------
 */

bool
VpxdSNMP::PackageBuilder::PushLong(int type,
                                   long value)
{
   long testvalue = (value < 0) ? -1 : 0;

   int length = GetSize();

   if (!PushByte((unsigned char)value)) {
      return false;
   }
   value >>= 8;

   while (value != testvalue) {
      if (!PushByte((unsigned char)value)) {
         return false;
      }
      value >>= 8;
   }

   unsigned char ch = GetBuffer()[0];
   if ((ch & 0x80) != (testvalue & 0x80)) {
      if (!PushByte((unsigned char)(testvalue & 0x80))) {
         return false;
      }
   }

   length = GetSize() - length;
   return PushHeader(type, length);
}


/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::PackageBuilder::PushSequence --
 *
 *    Push a sequence.
 *
 *    Package is built as
 *       [type] [length]
 *
 * Results:
 *    True on success, false on error.
 *
 *----------------------------------------------------------------------
 */

bool
VpxdSNMP::PackageBuilder::PushSequence(int type,
                                       int length)
{
   return PushHeader(type, length);
}


/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::PackageBuilder::PushHeader --
 *
 *    Build data header. 
 *    Header is consisted of type and length (of the data).
 *
 *    Package is built as
 *       [type] [length]
 *
 * Results:
 *    True on success, false on error.
 *
 *----------------------------------------------------------------------
 */

bool
VpxdSNMP::PackageBuilder::PushHeader(int type,
                                     int length)
{
   return PushLength(length) && PushByte(type);
}


/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::PackageBuilder::PushLength --
 *
 *    Save a length into package.
 *
 * Results:
 *    True on success, false on error.
 *
 *----------------------------------------------------------------------
 */

bool
VpxdSNMP::PackageBuilder::PushLength(int length)
{
   int size = GetSize();

   if (length <= 0x7F) {
      return PushByte((unsigned char)length);
   }

   while (length > 0xFF) {
      if (!PushByte((unsigned char)length)) {
         return false;
      }
      length >>= 8;
   }

   if (!PushByte((unsigned char)length)) {
      return false;
   }

   size = GetSize() - size;
   return PushByte((unsigned char)size | 0x80);
}


/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::PackageBuilder::PushByte --
 *
 *    Save a byte into package.
 *
 * Results:
 *    True on success, false on error.
 *
 *----------------------------------------------------------------------
 */

bool
VpxdSNMP::PackageBuilder::PushByte(unsigned char ch)
{
   if (m_available == 0) {
      return false;
   }

   m_buffer[--m_available] = ch;
   return true;
}

/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::PackageBuilder::PushUnsignedBlock --
 *
 *    Save a block of unsigned chars into package.
 *
 * Results:
 *    True on success, false on error.
 *
 *----------------------------------------------------------------------
 */

bool
VpxdSNMP::PackageBuilder::PushUnsignedBlock(const unsigned char *buf,
                                            int length)
{
   if (m_available < length) {
      return false;
   }
   m_available -= length;

   memmove(m_buffer + m_available, buf, length);
   return true;
}

/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::PackageBuilder::PushBlock --
 *
 *    Save a block of bytes into package.
 *
 * Results:
 *    True on success, false on error.
 *
 *----------------------------------------------------------------------
 */

bool
VpxdSNMP::PackageBuilder::PushBlock(const char *buf,
                                    int length)
{
   if (m_available < length) {
      return false;
   }
   m_available -= length;

   memmove(m_buffer + m_available, buf, length);
   return true;
}


/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::PackageBuilder::ValidateOID --
 *
 *    Check if an OID path is a right path.
 *    An oid path is a string seperated by dot.
 *
 * Results:
 *    True on success, false on error.
 *
 *----------------------------------------------------------------------
 */

bool
VpxdSNMP::PackageBuilder::ValidateOID(const string& oid)
{
   vector<int> nodes;
   return SplitDotString(oid, nodes);
}


/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::PackageBuilder::SplitDotString --
 *
 *    Read all nodes in an OID string and save them into a vector.
 *
 * Results:
 *    True on success, false on error.
 *
 *----------------------------------------------------------------------
 */

bool 
VpxdSNMP::PackageBuilder::SplitDotString(const string& oid, 
                                         vector<int>& nodes)
{
   string oidx = oid + ".";
   
   int begin = 0;
   int end;
   while ((end = oidx.find(".", begin)) != string::npos) {
      string nodestr = oidx.substr(begin, end);
      begin = end + 1;

      int node = atoi(nodestr.c_str());
      nodes.push_back(node);
   }
   return true;
}


/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::Session::Session --
 *
 *    Constructor.
 *
 *----------------------------------------------------------------------
 */

VpxdSNMP::Session::Session()
   : m_version(ASN_TRAP_SNMPV1), m_port(ASN_TRAP_PORT), 
   m_traptype(0), m_timestamp(0)
{
}


/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::Pdu::~Pdu --
 *
 *    Destructor. Realease all variables in it.
 *
 *----------------------------------------------------------------------
 */

VpxdSNMP::Pdu::~Pdu()
{
   for (int i = 0; i < m_variables.size(); i++) {
      PduVar *var = m_variables[i];
      delete var;
   }
}


/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::Pdu::AddVar --
 *
 *    Add pdu string variable.
 *
 * Results:
 *    True on success, false on error.
 *
 *----------------------------------------------------------------------
 */

bool
VpxdSNMP::Pdu::AddVar(const string& oid,
                      const string& value,
                      int maxlen)
{
   if (!PackageBuilder::ValidateOID(oid)) {
      return false;
   }

   PduVarString *var;
   if (maxlen > 0 && value.length() > maxlen) {
      // truncate value to maxlen
      string pushval = VpxdUtil_TruncateUtfString(value, maxlen);
      var = new PduVarString(oid, pushval);
   } else {
      var = new PduVarString(oid, value);
   }
   m_variables.push_back(var);

   return true;
}


/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::Pdu::AddVar --
 *
 *    Overload to add pdu int variable.
 *
 * Results:
 *    True on success, false on error.
 *
 *----------------------------------------------------------------------
 */

bool
VpxdSNMP::Pdu::AddVar(const string& oid,
                      const int& value)
{
   if (!PackageBuilder::ValidateOID(oid)) {
      return false;
   }

   PduVarInteger *var = new PduVarInteger(oid, value);
   m_variables.push_back(var);

   return true;
}


/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::Pdu::AddVar --
 *
 *    Overload to add pdu DateAndTime variable.
 *
 * Results:
 *    True on success, false on error.
 *
 *----------------------------------------------------------------------
 */

bool
VpxdSNMP::Pdu::AddVar(const string& oid,
                      const DateTime& value)
{
   if (!PackageBuilder::ValidateOID(oid)) {
      return false;
   }

   PduVarDateTime *var = new PduVarDateTime(oid, value);
   m_variables.push_back(var);

   return true;
}


/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::Init --
 *
 *    Initialize the SNMP module.
 *
 * Results:
 *    True on success, false on error.
 *
 *----------------------------------------------------------------------
 */

bool
VpxdSNMP::Init()
{
  //Do the managed object initialization.
   SnmpSystemMo::Init();

   _sSnmpColdStartTrapEnable = VpxConfig_GetBool(true, SNMP_CONFIG_CS_EN);
   _sSnmpPeriodicTestTrapEnable = VpxConfig_GetBool(false, SNMP_CONFIG_TT_EN);
   _sSnmpTestTrapInterval = VpxConfig_GetLong(300,  SNMP_CONFIG_TT_PER);

   /*
    * Sends cold start trap and test traps, scheduled to run every 
    * _sSnmpTestTrapInterval seconds
    */

   if (_sSnmpColdStartTrapEnable && !SendColdStartTrap()) {
      VmacoreLog(vpxservicesvpxdSnmpLogger, Vmacore::Service::Logger::error, "[VpxdSNMP::Init] ColdStartTrap sending failed." );
   }
   if (_sSnmpPeriodicTestTrapEnable)
   {
      Ref<ScheduledItem> si;
      VpxUtil_ScheduleTimerWrapper(MakeFunctor(&SendPeriodicTestTrap),
         _sSnmpTestTrapInterval* 1000 * 1000, si);
   }
   return true;
}


/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::Exit --
 *
 *    Finalize the SNMP module.
 *
 * Results:
 *    True on success, false on error.
 *
 *----------------------------------------------------------------------
 */

void
VpxdSNMP::Exit()
{
}


/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::SendTrap --
 *
 *    Send a SNMP trap with specified parameters.
 *
 * Results:
 *    True on success, false on error.
 *
 *----------------------------------------------------------------------
 */

bool
VpxdSNMP::SendTrap(const Session& session,
                   const Pdu& pdu,
                   int specialTrapType)
{
   PackageBuilder builder;
   if (!BuildPackage(builder, session, pdu, specialTrapType)) {
      return false;
   }

   return SendPackage(builder, session);
}


/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::BuildPackage --
 *
 *    Build SNMP trap package.
 *
 *    Package is build as
 *       [version] [community] [PDU type(4)] [enterprise] [agent address]
 *       [trap type(6)] [specific code] [time stamp]
 *       [var1 name] [var1 data] [var2 name] [var2 data] ...
 *
 *    NOTES:
 *       To make the package building efficient, the package is built
 *       from rear to front.
 *
 * Results:
 *    True on success, false on error.
 *
 *----------------------------------------------------------------------
 */

bool
VpxdSNMP::BuildPackage(PackageBuilder& builder,
                       const Session& session,
                       const Pdu& pdu,
                       int specialTrapType)
{
   if (session.m_version != 1) {
      return false;
   } else {
      // Build pdu.
      if (!pdu.BuildPackage(builder)) {
         return false;
      }

      int snmpTrapType;
      if (specialTrapType != SNMP_COLDSTART_TRAP) {
         snmpTrapType = ASN_TRAP_GENERIC_ENT_TYPE; //Test, Alarm, Event traps
      } else {
         snmpTrapType = ASN_TRAP_GENERIC_CS_TYPE; // Cold start traps
      }

      // Build trap header
      if (!builder.PushUnLong(ASN_TYPE_TIME, session.m_timestamp) ||
          !builder.PushLong(ASN_TYPE_INTEGER, session.m_traptype) ||
          !builder.PushLong(ASN_TYPE_INTEGER, snmpTrapType) ||
          !builder.PushIP(ASN_TYPE_IP, session.m_agent) ||
          !builder.PushOID(ASN_TYPE_OID, session.m_enterprise)) {
         return false;
      }
      int trapheader = builder.GetSize();
      if (!builder.PushSequence(ASN_TYPE_TRAP, trapheader)) {
         return false;
      }

      // Build snmp header
      if (!builder.PushString(ASN_TYPE_STRING, session.m_community) ||
         !builder.PushLong(ASN_TYPE_INTEGER, session.m_version - 1)) {
         return false;
      }
      int snmpheader = builder.GetSize();
      if (!builder.PushSequence(ASN_TYPE_SEQUENCE, snmpheader)) {
         return false;
      }
   }

   return true;
}


/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::SendPackage --
 *
 *    Send the SNMP trap package through UDP/IP.
 *
 * Results:
 *    True on success, false on error.
 *
 *----------------------------------------------------------------------
 */

bool
VpxdSNMP::SendPackage(const PackageBuilder& builder,
                      const Session& session)
{
   struct hostent *h = gethostbyname(session.m_destination.c_str());
   if (h == NULL) {
      return false;
   }

   int s = socket(AF_INET, SOCK_DGRAM, 0);
   if (!VpxdOsLayer::IsValidSocket(s)) {
      return false;
   }

   struct sockaddr_in to = {0};
   to.sin_family = AF_INET;
   to.sin_port   = htons(session.m_port);
   to.sin_addr   = *(struct in_addr *)h->h_addr;

   const char *buffer = (const char *)builder.GetBuffer();
   int length = builder.GetSize();
   int result = sendto(s, buffer, length, 0, (struct sockaddr*)&to, sizeof(to));
   VpxdOsLayer::CloseSocket(s);

   return result >= 0;
}


/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::SendColdStartTrap --
 *
 *    Send a SNMP cold start trap.
 *
 * Results:
 *    True if sent a cold start trap.
 *    False if SendSpecialTrap failed.
 *
 * Side effects:
 *    None.
 *
 *----------------------------------------------------------------------
 */
 
bool
VpxdSNMP::SendColdStartTrap()
{
   if (SendSpecialTrap(SNMP_COLDSTART_TRAP)) {
      VmacoreLog(vpxservicesvpxdSnmpLogger, Vmacore::Service::Logger::verbose, "[VpxdSNMP::SendColdStartTrap] Cold start trap send succeeded.");
      return true;
   } else {
      VmacoreLog(vpxservicesvpxdSnmpLogger, Vmacore::Service::Logger::error, "[VpxdSNMP::SendColdStartTrap] Cold start trap send failed.");
      return false;
   }

}

/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::SendPeriodicTestTrap --
 *
 *    Send periodic SNMP test traps.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None.
 *
 *----------------------------------------------------------------------
 */
 
void
VpxdSNMP::SendPeriodicTestTrap()
{
   SendTestTrap();

   Ref<ScheduledItem> si;
   VpxUtil_ScheduleTimerWrapper(MakeFunctor(&VpxdSNMP::SendPeriodicTestTrap),
      _sSnmpTestTrapInterval * 1000 * 1000, si);
}
   

/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::SendTestTrap --
 *
 *    Send a SNMP test trap.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None.
 *
 *----------------------------------------------------------------------
 */
 
void
VpxdSNMP::SendTestTrap()
{
   
   if (SendSpecialTrap(SNMP_TEST_TRAP)) {
      VmacoreLog(vpxservicesvpxdSnmpLogger, Vmacore::Service::Logger::verbose, "[VpxdSNMP::SendTestTrap] Test trap send succeeded.");
   }
   else {
      VmacoreLog(vpxservicesvpxdSnmpLogger, Vmacore::Service::Logger::error, "[VpxdSNMP::SendTestTrap] Test trap send failed.");
   }
}


/*
 *----------------------------------------------------------------------
 *
 * VpxdSNMP::SendSpecialTrap --
 *
 *    Send a special SNMP trap ( either cold start or test )
 *
 * Results:
 *    True when succeeds to send the designated trap.
 *    False when fails to send the designated trap.
 *
 * Side effects:
 *    None.
 *
 *----------------------------------------------------------------------
 */

bool
VpxdSNMP::SendSpecialTrap(int specialTrapType)
{
   VpxdSNMP::Pdu pdu;
   VpxdSNMP::Session session;
   bool result = true;

   string agentname = VpxUtil_GetLocalHostname();
   session.m_enterprise = SNMP_TRAP_OID;
   session.m_agent      = agentname;
   session.m_traptype   = ASN_TRAP_SPECIFIC_TEST;
   
   session.m_timestamp  = VpxdOsLayer::GetTickCount() / 10;

   
   for (int index = 1; index <= SNMP_PATH_RECEIVER_MAX; index++) {

      string path = string(SNMP_PATH_RECEIVER) + VpxUtil_Int2Str(index);
      bool enabled;
      int port;
      string destination;
      string community;

      VpxdConfig::GetValue(path + ".enabled", enabled);
      VpxdConfig::GetValue(path + ".port", port);
      VpxdConfig::GetValue(path + ".name", destination);
      VpxdConfig::GetValue(path + ".community", community);

      if (!enabled || destination.empty() || community.empty()) {
         continue;
      }

      session.m_destination = destination;
      session.m_community   = community;
      session.m_port        = port;

      if (!VpxdSNMP::SendTrap(session, pdu, specialTrapType)) {
        result = false;
      }
   }
   return result;
}


/*
 *----------------------------------------------------------------------
 *
 * SnmpSystemMo::Init --
 *
 *    Initialize the SnmpSystemMo singleton.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None.
 *
 *----------------------------------------------------------------------
 */

void VpxdSNMP::SnmpSystemMo::Init()
{
   _instance = new SnmpSystemMo("SnmpSystem");
   RegisterManagedObject(_instance);
   ASSERT(_instance != NULL);
}


/*
 *----------------------------------------------------------------------
 *
 * SnmpSystemMo::SnmpSystemMo --
 *
 *    Create an instance of the vmomi singleton object.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None.
 *
 *----------------------------------------------------------------------
 */

VpxdSNMP::SnmpSystemMo::SnmpSystemMo(const string& id) // IN: Object id
   : VpxdManagedSingleton(id, LOCK_LEVEL_MODULE3)
{
   //Define the agent limits for this agent.
   //Vpxd currently supports only test traps from the API.

    localAgentLimits = new ::Vim::Host::SnmpSystem::AgentLimits();
    localAgentLimits->SetCapability(Vim::Host::SnmpSystem::AgentLimits::DIAGNOSTICS);
}


/*
 *----------------------------------------------------------------------
 *
 * SnmpSystemMo::GetConfiguration --
 *
 *    Returns current configuration. 
 *    Not supported as of now in vpxd.
 *
 * Results:
 *    Returns a SnmpConfigSpec object depicting the current configuration.
 *
 * Side effects:
 *    None.
 *
 *----------------------------------------------------------------------
 */

void VpxdSNMP::SnmpSystemMo::GetConfiguration
     (Ref< ::Vim::Host::SnmpSystem::SnmpConfigSpec >& result /* OUT */)
{
   result = NULL;
}


/*
 *----------------------------------------------------------------------
 *
 * SnmpSystemMo::GetLimits --
 *
 *    Returns current agent-specific limits. 
 *
 * Results:
 *    Returns a AgentLimits object depicting the current agent limits.
 *
 * Side effects:
 *    None.
 *
 *----------------------------------------------------------------------
 */

void VpxdSNMP::SnmpSystemMo::GetLimits
         (Ref< ::Vim::Host::SnmpSystem::AgentLimits >& result /* OUT */)
{
  result = localAgentLimits;
}


/*
 *----------------------------------------------------------------------
 *
 * SnmpSystemMo::ReconfigureSnmpAgent --
 *
 *    Reconfigures the SNMP agent. 
 *    Not supported as of now in vpxd.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None.
 *
 *----------------------------------------------------------------------
 */

void VpxdSNMP::SnmpSystemMo::ReconfigureSnmpAgent
         (::Vim::Host::SnmpSystem::SnmpConfigSpec *spec)
{
  throw Vmomi::Fault::NotSupported::Exception();  
}


/*
 *----------------------------------------------------------------------
 *
 * SnmpSystemMo::SendTestNotification --
 *
 *    Sends a test notification.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None.
 *
 *----------------------------------------------------------------------
 */

void VpxdSNMP::SnmpSystemMo::SendTestNotification()
{
   VpxdSNMP::SendTestTrap();
}


