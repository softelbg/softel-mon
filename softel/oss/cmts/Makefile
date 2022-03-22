#
# (C) Copyright softel.bg 2012. All rights reserved.
#
#
# Description: This is the Makefile for the OSS CMTS Lib.
#
.SILENT:

include ../../makefile.pre

l = ../$(LIB)/lib$(LIBSFOSS).a
d = ../$(BIN)/lib$(LIBSFOSS).so

CFLAGS += $(INCPATH)

LDFLAGS += $(LIBPATH)

obj =   $(l)(CCmts.o) \
		$(l)(CCmtsCm.o) \
		$(l)(CSnmpOid.o) \
		$(l)(BSnmpOidBase.o) \
		$(l)(CCmtsToolsSql.o) \
		$(l)(CCmtsList.o) \
		$(l)(CSnmpToolsAsynch.o) \
		$(l)(CSnmpValue.o) \
		$(l)(CSnmpValueSet.o) \
		$(l)(CSnmpValueDocs3.o) \
		$(l)(CSnmpValueDocs3Set.o) \
		$(l)(CCmtsCmCpeList.o) \
		$(l)(CCpe.o) \
		$(l)(CCmtsIf.o) \
		$(l)(CCmtsModProfile.o) \
		$(l)(CCmtsModProfilesList.o) \
		$(l)(CCmtsElement.o) \
		$(l)(CBaseList.o) \
		$(l)(CCmtsCmList.o) \
		$(l)(CSqlToolsBase.o) \
		$(l)(CSnmpValueSpeed.o) \
		$(l)(CCmtsCmIfList.o) \
		$(l)(CAvgParamsIfFromCm.o) \
		$(l)(CCmtsFileIni.o) \
		$(l)(CCmtsLog.o) \
		$(l)(CCmtsAlertLog.o) \
		$(l)(CMySqlConn.o)
		
.cc.a .cc:
	@ echo --- c rule for $* --- $(CC) $(CFLAGS) -c $*.$(CCTYPE) 
	@ echo --- $(AR) $(ADD) $(l) $*.o
	if ( $(CC) $(CFLAGS) -c $*.$(CCTYPE) ) ; \
	then echo "compilation successful"; \
	$(AR) $(ADD) $(l) $*.o; \
	else echo "ERROR occurred in $*" ; \
	echo "delete older $*.o from archive" ; \
	$(AR) $(DELETE) $(l) $*.o ; \
	fi


all:   $(obj)


link:
	@ echo "--- create $(d) ---"
	@ echo --- $(LD) *.o $(l)  \
	-L$(LIB) \
	$(LDFLAGS) \
	$(LFLAGS) \
	-o $(d)
	$(LD) *.o $(l)  \
	-L$(LIB) \
	$(LDFLAGS) \
	$(LFLAGS) \
	-o $(d)


clean:
	@ echo --- cleanup
	-rm *.o
	-rm $(d)
	-rm $(l)

