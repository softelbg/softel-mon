#ifndef ErrorMsg_H
#define ErrorMsg_H

// Class CIniFile
const char
FEOPENINIFILE[] = "Fatal error : Can't open INI file !";

// Class CIp
const char
FEOPENFPINGSRC[] = "Fatal error : Can't open fpingsrc.txt !",
FEOPENFPINGRES[] = "Fatal error : Can't open fpingres.txt !",
FEOPENFPINGSRCORRES[] = "Fatal error : Can't open fpingsrc.txt or fpingres.txt!",
FEEXECFPING[] = "Fatal error : Can't execute ping !",
FEREADSNMP[] = "Fatal error : Can't read snmp !",
  FEBADVALUESNMP[] = "Fatal error : Bad snmp value !",
FEOPENSNMP[] = "Fatal error : Can't open snmp session handle!",
FEEXECSNMP[] = "Fatal error : Can't execute snmp !";

// Class CListNode
const char
FEALLOCATEMEMFORLN[] = "Fatal error : Can't allocate mem for ListNode !";

// Class CSharedCounter
const char
FEFAILEDSHMGET[] = "Fatal error : Failed shmget() !",
FEFAILEDSHMAT[] = "Fatal error : Failed shmat() !",
FEFAILEDSHMCTL[] = "Fatal error : Failed shmctl() !";

// Main SysMon
const char
FETIMEOUTMAIN[] = "Fatal error : Main process STOP by reason of TimeOut !",
FETIMEOUTCHILD[] = "Fatal error : Child process STOP by reason of TimeOut !",
FEFAILEDFORK[] = "Fatal error : Failed to fork !",
FEFAILEDGETSNMP[] = "Failed to get snmp !",
FEFAILEDSETSNMP[] = "Failed to set snmp !",
FEWRONGSNMPVALUE[] = "Wrong snmp value !";

// CMTSMon
const char
ERR_FORK[] = "Fork Failed!";


#endif
