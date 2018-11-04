# Example Makefile exploring ways to use for loops in Makefiles.
# Kees Lemmens, Feb 2014

clean : clean1

dirs = $(shell find . -mindepth 1 -maxdepth 1 -type d)

define subdirmake
   echo $(subdir)
   @test -r $(subdir)/Makefile && cd $(subdir) && make allclean ; cd $(CURDIR)
endef

test:
	@echo $(dirs)

clean0:
	rm -vf *~ core
	rm -rf .codelite .clang

clean1: clean0
	$(foreach subdir,$(dirs), $(subdirmake) )
	
allclean : clean1
