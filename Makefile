all:	src/makefile 
	cd src;$(MAKE) all RATstart
	cd docs;$(MAKE) html


src/makefile:
	./configure

clean:	src/makefile
	@cd src;$(MAKE) clean

python:
	@cd src;$(MAKE) python

up:
	bin/csh/pushMe

down:
	bin/csh/pullYou

