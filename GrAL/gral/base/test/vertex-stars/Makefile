# gral/base/test/vertex-stars

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)


SRC  =  $(shell ls *.C)
OBJ  =  $(SRC:%.C=%.o)
DEPS =  $(SRC:%.C=%.d)
EXEC =  test-vertex-stars

include $(DEPS)

EXTRA_DEPMODULES+= gral/cartesian2d geometry

LIBS = -lgral-cartesian2d -lgral-base



new-check-clean: # new check clean1
	$(MAKE) new
	$(MAKE) check
	$(MAKE) clean

new : clean all


all : $(EXEC)


$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	@rm -f $(EXEC)
	@rm -f *.o *~ *.flc core
	@rm -f .depend *.oogl *out 


