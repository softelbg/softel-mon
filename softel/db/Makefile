#
# (C) Copyright Softel 2010. All rights reserved.
#
#
# Description: This is the Makefile for the OSS Data Base Tools
#
.SILENT:

include ../makefile.pre

l = $(LIB)/lib$(LIBSFDB).a
d = $(BIN)/lib$(LIBSFDB).so

CFLAGS += $(INCPATH)
CFLAGS += -I../../

LDFLAGS += $(LIBPATH)

LFLAGS += -lmysqlclient
LFLAGS += -lz
LFLAGS += -lnetsnmp
LFLAGS += -lcrypto
LFLAGS += -lm

obj =   $(l)(CMySqlDB.o) \
	$(l)(CSqlConnPool.o) \
	$(l)(CDataSet.o) \
	$(l)(CDataRow.o) \
	$(l)(CIndexedVector.o) \
	$(l)(CMapIndex.o) \
	$(l)(CPosIndex.o) \
	$(l)(CPosIndexList.o) \
	$(l)(CStringVectorIndexed.o) \
	$(l)(CParamSet.o) \
	$(l)(BDbSqlBase.o) \
	$(l)(ISqlDbInterface.o) \
	$(l)(CDbMySql.o) \
	$(l)(CDbPgSql.o) \
	$(l)(WorkItemQueryBase.o) \
	$(l)(WorkItemMySqlQuery.o) \
	$(l)(CSqlDbFactory.o)

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

