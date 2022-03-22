#
# (C) Copyright Softel 2010. All rights reserved.
#
#
# Description: This is the Makefile for the main Softel Infrastructure Management
#
.SILENT:

include ../makefile.pre

SUBDIRS = 	admtrade \
		../trademachine
		
EXECUTABLE = wthttpd
l = $(LIB)/lib$(EXECUTABLE).a
d = $(BIN)/$(EXECUTABLE)

CFLAGS = -Wall
CFLAGS += -std=c++11
CFLAGS += $(INCPATH)

LDFLAGS += -L/usr/local/lib/mysql/
LDFLAGS += -L/usr/lib64/mysql/
LDFLAGS += $(LIBPATH)
LDFLAGS += -L../../../lib/

LFLAGS += -l$(LIBSFTRADE)
LFLAGS += -l$(LIBSFCORE)
LFLAGS += -lwthttp -lwt

obj =		$(l)(EscrowWidget.o) \
			$(l)(TradingApplication.o) \
			$(l)(BaseTradeWidget.o) \
			$(l)(InfoWidget.o) \
			$(l)(UinfoStorage.o) \
			$(l)(UinfoWidget.o) \
			$(l)(InitMongoUsers.o) \
			$(l)(TradingWidget.o) \
			$(l)(ForexWidget.o) \
			$(l)(BitcoinMachine.o) \
			$(l)(BitcoinTradeWidget.o) \
			$(l)(ItemTradeWidget.o) \
			$(l)(TallentTradeWidget.o) \
			$(l)(MarketDataWidget.o) \
			$(l)(BaseOrderWidget.o) \
			$(l)(OrderMarketWidget.o) \
			$(l)(OrderForexWidget.o) \
			$(l)(OrderItemTradeWidget.o) \
			$(l)(OrderTallentWidget.o) \
			$(l)(MongoTable.o) \
			$(l)(BaseMachine.o) \
			$(l)(TradeMachine.o) \
			$(l)(ForexMachine.o) \
			$(l)(ItemTradeMachine.o) \
			$(l)(TallentMachine.o) \
			$(l)(wtmain.o)
		
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


all:   $(obj) $(EXECUTABLE) 
		for i in $(SUBDIRS); do \
        	cd "$$i" && $(MAKE) $@; \
    	done

$(EXECUTABLE): $(obj) 
	@ echo --- Build $(EXECUTABLE) 
	@ echo --- "$(CC) $(CFLAGS) $(INCPATH) *.o $(LDFLAGS) $(LFLAGS) -o $(d)"
	$(CC) $(CFLAGS) $(INCPATH) *.o $(LDFLAGS) $(LFLAGS) -o $(d)

link:
	

clean:
	@ echo --- cleanup
	-rm *.o
	-rm $(d)
	-rm $(l)
	for i in $(SUBDIRS); do \
       	cd "$$i" && $(MAKE) $@; \
	done
	
install: $(EXECUTABLE) 
	@ echo --- install
	cp $(d) /usr/local/sbin
	cp $(EXECUTABLE).ini /etc/

