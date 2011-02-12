# Makefile for loop_daemon.c

include ../Globals.mk

SOURCES=loop_daemon.c ../helper/logger.cpp

HEADERS=../helper/logger.h ../helper/systemState.h

OBJECTS=logger.o loop_daemon.o

INCLUDES=-I../helper/

loop_daemon: $(OBJECTS)
	$(CPP) $(OBJECTS) $(CP_LDFLAGS) -o loop_daemon

static: $(OBJECTS)
	@echo !! Static linking enabled !!
	$(CPP) $(STATIC_BASE) $(STATIC_CC) $(OBJECTS) \
		$(CP_LDFLAGS) -o loop_daemon

debug: $(SOURCES) $(HEADERS)
	@echo !! Debug build enabled !!
	$(CPP) $(DEBUG) $(CP_FLAGS) $(INCLUDES) $(SOURCES)
	$(CPP) $(OBJECTS) $(CP_LDFLAGS) -o loop_daemon

static-debug: $(SOURCES) $(HEADERS)
	@echo !! Debug with static linking enabled !!
	$(CPP) $(DEBUG) $(CP_FLAGS) $(INCLUDES) $(SOURCES)
	$(CPP) $(STATIC_BASE) $(STATIC_CC) $(OBJECTS) \
		$(CP_LDFLAGS) -o loop_daemon

logger.o: loop_daemon.o

loop_daemon.o: $(SOURCES) $(HEADERS)
	$(CPP) $(CP_FLAGS) $(INCLUDES) $(SOURCES)

.PHONY: clean

clean:
	@echo Cleaning the build environment
	@rm -f *~ *.o *.so core core.*
	@rm -f ../*~ ../helper/*~ ../helper/*.o
	@rm -f loop_daemon
