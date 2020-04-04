
all:	src/makefile	
	@cd src;make 

src/makefile:
	./configure

clean:	src/makefile
	@cd src;make clean

test:   src/makefile test_start.test.op

test_start.test.op:	src/makefile
	make all
	@bin/csh/test_start

start:	src/makefile
	@cd src;make start

sick:	src/makefile
	@echo "stick your fingers down your throat"
