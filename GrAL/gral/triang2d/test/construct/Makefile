# gral/triang2d/test/construct

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)


EXTRA_DEPMODULES +=  external-control utilities gral/testing

SOURCES =  test-triang2d-construct.C
OBJECTS =  $(SOURCES:%.C=%.o)
DEPS    =  $(SOURCES:%.C=%.d)
TARGETS = test-triang2d-construct
EXEC =  $(TARGETS)

include $(DEPS)



LIBS = -lgral-triang2d -lgral-base -lgeometry -lexternal-control -lsequence -lutilities



new-check-clean: new check clean

new : clean all


all : $(TARGETS)


$(EXEC): $(OBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 


