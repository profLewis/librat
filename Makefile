all:	src/makefile 
	cd src;$(MAKE) all RATstart
	
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
up:
	bin/csh/pushMe

pull:
down:
	bin/csh/pullYou

