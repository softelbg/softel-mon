#
# (C) Copyright Softel (softel.bg) 2012. All rights reserved.
#
#
# Description: This is the Makefile for Softel libs
#
.SILENT:

SHELL=/bin/sh

SUBDIRS = 	core \
		../db \
		../sim \
		../mon \
		../dsp \
		../trade \
		../wt \
		../ml \
		../exec

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

trd:
	@ echo --- Trade Clean Build
	@ cd trade; gmake clean all
	@ cd wt; gmake clean all

recursive: true

