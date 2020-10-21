all:	src/makefile 
	cd src;$(MAKE) all RATstart test install
	
html:
	cd docs;$(MAKE) html
	
pdf:
	cd docs;$(MAKE) latexpdf


src/makefile:
	./configure

clean:	src/makefile
	@cd src;$(MAKE) clean

python:
	@cd src;$(MAKE) python

push:
	bin/csh/pushMe

up:
	bin/csh/pushMe

pull:
	bin/csh/pullYou

down:
	bin/csh/pullYou

