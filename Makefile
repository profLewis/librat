all:	src/makefile 
	cd src;$(MAKE) all ratstart
	cd docs;$(MAKE) html


src/makefile:
	./configure

clean:	src/makefile
	@cd src;$(MAKE) clean

python:
	@cd src;$(MAKE) python
