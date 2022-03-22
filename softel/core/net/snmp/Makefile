#
# (C) Copyright Softel 2010. All rights reserved.
#
#
# Description: This is the Makefile for the OSS Network SNMP
#
.SILENT:

include ../../../makefile.pre

l = ../../$(LIB)/lib$(LIBSFCORE).a
d = ../../$(BIN)/lib$(LIBSFCORE).so

CFLAGS += $(INCPATH)

LDFLAGS += $(LIBPATH)

obj =   $(l)(SnmpPackageBuilder.o) \
		$(l)(SnmpPdu.o) \
		$(l)(SnmpPduVar.o) \
		$(l)(SnmpTrap.o) \
		$(l)(SnmpSession.o)

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

