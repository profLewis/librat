
all:	src/makefile	
	@cd src;make 

src/makefile:
	./configure
	@cd src;make

clean:
	@cd src;make clean

test:   test_start.test.op

test_start.test.op:
	make all
	@bin/csh/test_start

start:
	@cd src;make start

sick:
	@echo "stick your fingers down your throat"
