
all:	
	@cd src;make 

clean:
	@cd src;make clean

test:   test_start.test.op

test_start.test.op:
	make all
	@bin/csh/test_start


