all:	src/makefile	
	@cd src;$(MAKE) 

src/makefile:
	./configure

clean:	src/makefile
	@cd src;$(MAKE) clean

check:
test:	src/makefile test_start.test.op

start_:	src/makefile
	@cd src;$(MAKE) start

sick:	src/makefile
	@echo "stick your fingers down your throat"

test_start.test.op:
	@echo "test_start.test.op"
	
start:
	@echo "start"

distcheck:
	@echo "done"
