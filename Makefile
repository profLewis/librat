all:	src/makefile 
	@cd src;$(MAKE) all start

src/makefile:
	./configure

clean:	src/makefile
	@cd src;$(MAKE) clean

check:
test:	src/makefile test_start.test.op

start:	src/makefile
	@cd src;$(MAKE) start

sick:	src/makefile
	@echo "stick your fingers down your throat"

test_start.test.op:
	@echo "test_start.test.op"
	

distcheck:
	@echo "done"
