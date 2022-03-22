/* **************************************************************************
 * Copyright 2003 VMware, Inc.  All rights reserved. -- VMware Confidential
 * **************************************************************************/

/*
 * vpxdSnmp.h --
 *
 *      Sends and receives SNMP traps.
 *
 */

#ifndef _VPXD_SNMP_H_
#define _VPXD_SNMP_H_

#include <vector>
#include <vmacore/system/DateTime.h>
#ifndef _VMODL_VMODL_VIM_H_
   #include "Vmodl/Vim/Host/SnmpSystem.h"
#endif
#include "baseMo.h"


#define SNMP_PATH_RECEIVER        "snmp.receiver."
#define SNMP_ENTERPRISE           "1.3.6.1.4.1.6876"
#define SNMP_TRAP_OID             SNMP_ENTERPRISE ".4.3"

// Alarm MIB OIDs
#define SNMP_TRAP_OID_TYPE        SNMP_TRAP_OID ".301.0"
#define SNMP_TRAP_OID_HOSTNAME    SNMP_TRAP_OID ".302.0"
#define SNMP_TRAP_OID_VMNAME      SNMP_TRAP_OID ".303.0"
#define SNMP_TRAP_OID_OLDCOLOR    SNMP_TRAP_OID ".304.0"
#define SNMP_TRAP_OID_OLDCOLOR_MAXLEN               255
#define SNMP_TRAP_OID_NEWCOLOR    SNMP_TRAP_OID ".305.0"
#define SNMP_TRAP_OID_NEWCOLOR_MAXLEN               255
#define SNMP_TRAP_OID_OBJVALUE    SNMP_TRAP_OID ".306.0"
#define SNMP_TRAP_OID_OBJVALUE_MAXLEN              4096
#define SNMP_TRAP_OID_TARGETOBJ   SNMP_TRAP_OID ".307.0"
#define SNMP_TRAP_OID_TARGETOBJ_MAXLEN              255
#define SNMP_TRAP_OID_TARGETTYPE  SNMP_TRAP_OID ".308.0"
#define SNMP_TRAP_OID_TARGETTYPE_MAXLEN             255

// Event MIB OIDs
#define SNMP_TRAP_OID_EVENT_ID          SNMP_TRAP_OID ".401.0"
#define SNMP_TRAP_OID_EVENT_TYPEID      SNMP_TRAP_OID ".402.0"
#define SNMP_TRAP_OID_EVENT_CREATEDTIME SNMP_TRAP_OID ".403.0"
#define SNMP_TRAP_OID_EVENT_MESSAGE     SNMP_TRAP_OID ".404.0"
#define SNMP_TRAP_OID_EVENT_OBJECTNAME  SNMP_TRAP_OID ".405.0"
#define SNMP_TRAP_OID_EVENT_OBJECTID    SNMP_TRAP_OID ".406.0"
#define SNMP_TRAP_OID_EVENT_OBJECTTYPE  SNMP_TRAP_OID ".407.0"
#define SNMP_TRAP_OID_EVENT_HOST        SNMP_TRAP_OID ".408.0"
#define SNMP_TRAP_OID_EVENT_HOSTID      SNMP_TRAP_OID ".409.0"
#define SNMP_TRAP_OID_EVENT_CRNAME      SNMP_TRAP_OID ".410.0"
#define SNMP_TRAP_OID_EVENT_CRID        SNMP_TRAP_OID ".411.0"
#define SNMP_TRAP_OID_EVENT_DCNAME      SNMP_TRAP_OID ".412.0"
#define SNMP_TRAP_OID_EVENT_DCID        SNMP_TRAP_OID ".413.0"
#define SNMP_TRAP_OID_EVENT_SEVERITY    SNMP_TRAP_OID ".414.0"
#define SNMP_TRAP_OID_EVENT_ARGS        SNMP_TRAP_OID ".415.0"
#define SNMP_TRAP_OID_EVENT_USERNAME    SNMP_TRAP_OID ".416.0"
#define SNMP_TRAP_OID_EVENT_FAULT       SNMP_TRAP_OID ".417.0"

#define SNMP_TRAPTYPE_HOST        2
#define SNMP_TRAPTYPE_VM          3
#define SNMP_TRAPTYPE_OTHER       4


/*
 * List of trap types supported  - public, so that SendTrap can be suitably
 * called from outside VpxdSNMP
 */

static const int  SNMP_COLDSTART_TRAP  = 0;
static const int  SNMP_TEST_TRAP       = 1;
static const int  SNMP_ALARM_TRAP      = 2;
static const int  SNMP_EVENT_TRAP      = 3;

static const int  SNMP_PATH_RECEIVER_MAX = 4;      // Max num of SNMP dests

/*Trap types we currently support*/

static const int  ASN_TRAP_GENERIC_CS_TYPE  = 0;   // Generic Cold Start trap
static const int  ASN_TRAP_GENERIC_ENT_TYPE = 6;   // Generic Enterprise-specific trap
static const int  ASN_TRAP_SPECIFIC_ALARM   = 203; // Specific Alarm type
static const int  ASN_TRAP_SPECIFIC_TEST    = 202; // Specific Test type
static const int  ASN_TRAP_SPECIFIC_EVENT   = 204; // Specific Event type

/* SNMP DateAndTime length in octets */
static const int SNMP_DATETIME_LEN = 11;

/*
 *-----------------------------------------------------------------------------
 *
 * class VpxdSNMP --
 *
 *    Provides SNMP trap interfaces. 
 *
 *    NOTE: This version supports SNMPv1 only. It could be enhanced to support
 *    SNMPv2c easily.    
 *
 *-----------------------------------------------------------------------------
 */

class VpxdSNMP
{
private:
   /*
    * Private constant members.
    */

   static const int  SNMP_BUFFER_SIZE  = 0x2000; // 8k

   static const int  ASN_TYPE_INTEGER  = 0x02;
   static const int  ASN_TYPE_STRING   = 0x04;
   static const int  ASN_TYPE_OID      = 0x06;
   static const int  ASN_TYPE_SEQUENCE = 0x30;
   static const int  ASN_TYPE_IP       = 0x40;
   static const int  ASN_TYPE_TIME     = 0x43;
   static const int  ASN_TYPE_TRAP     = 0xA4;

   static const int  ASN_TRAP_PORT     = 162;
   static const int  ASN_TRAP_SNMPV1   = 1;

   /*
    *---------------------------------------------------------------------
    *
    * class PackageBuilder --
    *
    *    Build the SNMP trap package which will be sent through UDP/IP.
    *
    *    The package is built from rear to front in order to make it
    *    efficient.
    *
    *---------------------------------------------------------------------
    */

   class PackageBuilder
   {
   public:
      PackageBuilder();

      const unsigned char *GetBuffer() const { return m_buffer + m_available; }
      int                  GetSize()   const { return sizeof(m_buffer) -  m_available; }

      bool  PushOID(int type, const std::string& oid);
      bool  PushIP(int type, const std::string& ip);
      bool  PushString(int type, const std::string& str);
      bool  PushUnLong(int type, unsigned long value);
      bool  PushLong(int type, long value);
      bool  PushSequence(int type, int length);
      bool  PushDateTime(int type, const Vmomi::DateTime& dateTime);

      static bool ValidateOID(const std::string& oid);
      static bool SplitDotString(const std::string& oid, vector<int>& nodes);

   private:
      bool  PushHeader(int type, int length);
      bool  PushLength(int length);
      bool  PushByte(unsigned char ch);
      bool  PushBlock(const char *buf, int length);
      bool  PushUnsignedBlock(const unsigned char *buf, int length);

      unsigned char  m_buffer[SNMP_BUFFER_SIZE];
      int            m_available;
   };

   /*
    *---------------------------------------------------------------------
    *
    * class PduVar --
    *    and inherited PduVarString, PduVarInt, PduVarTime
    *
    *    Define the base class of PDU variables. The PDU variable will
    *    be built in package as
    *       [sequence] [length] [ojbect id (name)] [object value (data]
    *
    *    NOTE: Currently we only support string variables.
    *
    *---------------------------------------------------------------------
    */

   class PduVar
   {
   public:
      PduVar(const std::string& oid) : m_oid(oid) {}
      virtual ~PduVar() {}

      virtual bool BuildPackage(PackageBuilder& builder)
      {
         int length = builder.GetSize();
         if (!BuildData(builder) || !BuildName(builder)) {
            return false;
         }
         length = builder.GetSize() - length;
         return builder.PushSequence(ASN_TYPE_SEQUENCE, length);
      }

      virtual bool BuildName(PackageBuilder& builder)
      { 
         return builder.PushOID(ASN_TYPE_OID, m_oid); 
      }

      virtual bool BuildData(PackageBuilder& builder) = 0;

   protected:
      std::string m_oid;
   };

   /*
    *---------------------------------------------------------------------
    *
    * class PduVarString --
    *
    *    String overload for a SNMP PDU varbind.
    *
    *---------------------------------------------------------------------
    */

   class PduVarString : public PduVar
   {
   public:
      PduVarString(const std::string& oid, const std::string& str) : PduVar(oid), m_str(str) {}

      virtual bool BuildData(PackageBuilder& builder) 
      { 
         return builder.PushString(ASN_TYPE_STRING, m_str); 
      }

   private:
      std::string m_str;
   };

   /*
    *---------------------------------------------------------------------
    *
    * class PduVarInteger --
    *
    *    Integer overload for a SNMP PDU varbind.
    *
    *---------------------------------------------------------------------
    */

   class PduVarInteger : public PduVar
   {
   public:
      PduVarInteger(const std::string& oid, const int& var) : PduVar(oid), m_var(var) {}

      virtual bool BuildData(PackageBuilder& builder)
      {
         return builder.PushLong(ASN_TYPE_INTEGER, m_var);
      }

   private:
      int m_var;
   };

   /*
    *---------------------------------------------------------------------
    *
    * class PduVarDateTime --
    *
    *    Vmomi::DateTime overload for a SNMP PDU varbind.
    *
    *---------------------------------------------------------------------
    */

   class PduVarDateTime : public PduVar
   {
   public:
      PduVarDateTime(const std::string& oid, const Vmomi::DateTime& var) : PduVar(oid), m_var(var) {}

      virtual bool BuildData(PackageBuilder& builder)
      {
         return builder.PushDateTime(ASN_TYPE_STRING, m_var);
      }

   private:
       Vmomi::DateTime m_var;
   };

public:
   /*
    *---------------------------------------------------------------------
    *
    * class Session --
    *
    *    Define interfaces of SNMP session.
    *
    *---------------------------------------------------------------------
    */

   class Session
   {
   public:
      Session();

      long     m_version;        // SNMP version. 1-> SNMPv1, 2-> SNMPv2

      std::string   m_destination;    // where to send the trap
      std::string   m_community;      // community on destination
      long     m_port;           // port number on destination

      std::string   m_enterprise;
      std::string   m_agent;
      long     m_traptype;
      long     m_timestamp;
   };

   /*
    *---------------------------------------------------------------------
    *
    * class Pdu --
    *
    *    Define interfaces of SNMP PDU. It keeps a list of PDU variables.
    *
    *---------------------------------------------------------------------
    */

   class Pdu
   {
   public:
      ~Pdu();
      bool AddVar(const std::string& oid, const std::string& value, int maxlen);
      bool AddVar(const std::string& oid, const int& value);
      bool AddVar(const std::string& oid, const Vmomi::DateTime& value);

      bool BuildPackage(PackageBuilder& builder) const
      {
         int length = builder.GetSize();
         for (int i = m_variables.size() - 1; i >= 0; i--) {
            PduVar *var = m_variables[i];
            if (!var->BuildPackage(builder)) {
               return false;
            }
         }
         length = builder.GetSize() - length;
         return builder.PushSequence(ASN_TYPE_SEQUENCE, length);
      }

   private:
      std::vector<PduVar*> m_variables;
   };
   
   
   /*
    *-----------------------------------------------------------------------------
    *
    * class SnmpSystemMo --
    *
    *    Provides external SNMP interface.
    *
    *    NOTE: This version supports SNMPv1 only. It could be enhanced to support
    *    SNMPv2c easily.    
    *
    *-----------------------------------------------------------------------------
    */

   class SnmpSystemMo : virtual public ::Vim::Host::SnmpSystem, 
                            public VpxdManagedSingleton {
     
     friend class VpxdSNMP;

   private:
     SnmpSystemMo(const std::string& id);
     
   public:
      void GetConfiguration
               (Ref< ::Vim::Host::SnmpSystem::SnmpConfigSpec >& result /* OUT */);
      void GetLimits(Ref< ::Vim::Host::SnmpSystem::AgentLimits >& result /* OUT */);
      void ReconfigureSnmpAgent(::Vim::Host::SnmpSystem::SnmpConfigSpec *spec);
      void SendTestNotification();
   
   public:
      static Ref<SnmpSystemMo> GetInstance() 
      {
         return _instance; 
      }

      static void Init();

   private:
      //AgentLimits for this agent.
      Ref< ::Vim::Host::SnmpSystem::AgentLimits> localAgentLimits;
      //Singleton
      static Ref<SnmpSystemMo> _instance;

   };
   /*
    * Module initialization and finalization.
    */

   static bool Init(void);
   static void Exit(void);

   /*
    * Send SNMP trap.
    */

   static bool SendTrap(const Session& session, const Pdu& pdu, int trapType);

   /*
    * SNMP specific trap functions.
    */

   static void SendPeriodicTestTrap();
   static void SendTestTrap();
   static bool SendColdStartTrap();

private:
   /*
    * Helper interfaces.
    */

   static bool BuildPackage(PackageBuilder& builder, 
                             const Session& session, 
                             const Pdu& pdu, 
                             int trapType);
   static bool SendPackage(const PackageBuilder& builder, const Session& session);
   static bool SendSpecialTrap(int specialTrapType);
   
   
   /*
    * Config file settings.
    */
 
   static bool _sSnmpPeriodicTestTrapEnable;         // Test traps enable
   static bool _sSnmpColdStartTrapEnable;            // Cold start traps enable
   static int  _sSnmpTestTrapInterval;               // Test traps periodic interval
 

};


#endif // _VPXD_SNMP_H_
