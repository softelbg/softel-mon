#
# (C) Copyright Softel 2010. All rights reserved.
#
#
# Description: This is the Makefile for the Softel Common Tools
#
.SILENT:

include ../../makefile.pre

l = ../$(LIB)/lib$(LIBSFCORE).a
d = ../$(BIN)/lib$(LIBSFCORE).so

CFLAGS += $(INCPATH)

LDFLAGS += $(LIBPATH)

obj =   $(l)(CStringUtil.o) \
		$(l)(CSharedCounter.o) \
		$(l)(CThreads.o) \
		$(l)(CThreadsAttributes.o) \
		$(l)(CTimeCounter.o) \
		$(l)(CTimeCounterFine.o) \
		$(l)(CScheduler.o) \
		$(l)(CWorkerBase.o) \
		$(l)(CWorkerJob.o) \
		$(l)(CThreadPoolBase.o) \
		$(l)(BFileIniBase.o) \
		$(l)(BSingletonBase.o) \
		$(l)(BFileBase.o) \
		$(l)(BLogFileBase.o) \
		$(l)(DateTime.o) \
		$(l)(PtrVector.o) \
		$(l)(Options.o) \
		$(l)(TableBase.o) \
		$(l)(TextFile.o) \
		$(l)(ModuleBase.o) \
		$(l)(SymbolMap.o) \
		$(l)(BFileReaderBase.o)

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

