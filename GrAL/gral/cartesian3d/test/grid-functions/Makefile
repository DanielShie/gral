# gral/cartesian3d/test/iterators

PWD=$(shell pwd)
CONFIG=$(PWD)/../../local.config
include $(CONFIG)


LOCALSOURCES =  test-cart-gf.C
LOCALOBJECTS =  $(LOCALSOURCES:%.C=%.o)
DEPS = $(LOCALSOURCES:%.C=%.d)
TARGETS = test-cart-gf
EXEC =  $(TARGETS)

include $(DEPS)



LIBS = -lgral-cartesian3d -lgral-complex2d -lgral-base





new-check-clean: new check clean

new : clean all


all : $(TARGETS)


test-cart-gf: $(LOCALOBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(LOCALOBJECTS) $(LDFLAGS) $(LIBS)

run:
	$(EXEC) 

check: run-check


clean: stdclean
	rm -f $(EXEC)
	rm -f *.o *~ *.flc core
	rm -f .depend *.oogl *.out 


