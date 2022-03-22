#
# (C) Copyright Softel (softel.bg) 2012. All rights reserved.
#
#
# Description: This is the Makefile for the Softel Monitoring Library
#
.SILENT:

include ../makefile.pre

SUBDIRS = 	core \
			../event

all: 
	for i in $(SUBDIRS); do \
        	cd "$$i" && $(MAKE) $@; \
    	done	

clean:
	for i in $(SUBDIRS); do \
                cd "$$i" && $(MAKE) $@; \
        done


recursive: true

