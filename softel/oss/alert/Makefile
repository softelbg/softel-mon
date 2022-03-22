#
# (C) Copyright softel.bg 2012. All rights reserved.
#
#
# Description: This is the Makefile for the OSS Alert Module.
#
.SILENT:

include ../../makefile.pre

l = ../$(LIB)/lib$(LIBSFOSS).a
d = ../$(BIN)/lib$(LIBSFOSS).so

CFLAGS += $(INCPATH)

LDFLAGS += $(LIBPATH)

obj =   $(l)(COssAlert.o) \
		$(l)(CEventBase.o) \
		$(l)(CEventThreshold.o) \
		$(l)(CEventCmtsIf.o) \
		$(l)(CAlertCm.o) \
		$(l)(CSqlAlertTools.o) \
		$(l)(CSqlThresholdTools.o) \
		$(l)(CThreshold.o) \
		$(l)(CThresholdBase.o) \
		$(l)(CThresholdListRaw.o) \
		$(l)(CThresholdListRawCm.o) \
		$(l)(CThresholdListRawCmtsIf.o) \
		$(l)(CThresholdOid.o) \
		$(l)(CThresholdOidList.o) \
		$(l)(CThresholdListBase.o) \
		$(l)(CCmtsCmLatestRaw.o) \
		$(l)(CThresholdResult.o) \
		$(l)(CEventThresholdCmtsCm.o) \
		$(l)(CCmtsIfRawCmCounter.o) \
		$(l)(CCmtsIfCmCounter.o) \
		$(l)(CAlertListBase.o) \
		$(l)(CAlertListCmtsIfCmCount.o) \
		$(l)(CAlertCmtsIf.o) \
		$(l)(CAlertCmtsIfCmCount.o) \
		$(l)(CAlertCmDedicated.o)

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

