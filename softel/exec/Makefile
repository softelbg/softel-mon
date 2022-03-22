#
# (C) Copyright Softel (softel.bg) 2012. All rights reserved.
#
#
# Description: This is the Makefile for the OSS 
#
.SILENT:

SHELL=/bin/sh

SUBDIRS = 	WebSrv \
			../zmqsub

SUBDIRSTEST = Tester

all: 
	for i in $(SUBDIRS); do \
        	cd "$$i" && $(MAKE) $@; \
    	done	

tester:
	for i in $(SUBDIRSTEST); do \
		cd "$$i" && $(MAKE) $@; \
	done

clean:
	for i in $(SUBDIRS); do \
                cd "$$i" && $(MAKE) $@; \
        done


recursive: true

