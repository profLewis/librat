all:	src/makefile 
	@cd src;$(MAKE) clean all ratstart

src/makefile:
	./configure

clean:	src/makefile
	@cd src;$(MAKE) clean

python:
	@cd src;$(MAKE) python
