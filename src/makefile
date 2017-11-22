#    provided rules are:
#    all        builds the main program
#    test       builds the test program as test_program
#    clean      removes all files listed if inclusive, or all those which are not code, debug, programs, or explicitely named if exclusive
#    cleanDir   removes all subdirectories of curent directory, will prompt you to be sure.  Not functional under inclusive delete
#    cleanAll   removes all files listed if inclusive, or all those which are not code or explicitely named if exclusive, then calls cleanDir as well.
#    v          runs valgrind on the main program
#    vt         runs valgrind on the test program
#    data       copies the files listed in DATAFILES to the current dir, using DATAPATH to find them
#    update	grabs the current makefile from https://github.com/Athandreyal/Makefile/blob/master/makefile   -does not replace modules.mk, ONLY this file

#      _          _   _  ____ _______             _ _ _      _   _     _         __ _ _
#     | |        | \ | |/ __ \__   __|           | (_) |    | | | |   (_)       / _(_) |
#   __| | ___    |  \| | |  | | | |       ___  __| |_| |_   | |_| |__  _ ___   | |_ _| | ___
#  / _` |/ _ \   | . ` | |  | | | |      / _ \/ _` | | __|  | __| '_ \| / __|  |  _| | |/ _ \
# | (_| | (_) |  | |\  | |__| | | |     |  __/ (_| | | |_   | |_| | | | \__ \  | | | | |  __/
#  \__,_|\___/   |_| \_|\____/  |_|      \___|\__,_|_|\__|   \__|_| |_|_|___/  |_| |_|_|\___|
#

# Disclaimer:
#       1) I am not responsible for anything.  Your whole user directory goes poof? NOT. MY. PROBLEM
#
#
# TERMS OF USE:
#       1) This makefile, and associated modules.mk are provided as is.  Feel free to ask if something needs work, but I may or may not offer my time.
#       2) Modify it as you see fit, abuse and use.  Update the version history if you do so, so that those after you can give credit where due.


#
#  If you do edit this, update the version list below, and credit yourself as author of that update.
#

#ver 1  author: prenw499, Phillip Renwick, Q1 2016      initial
#ver 2  author: prenw499, Phillip Renwick, Q1 2016      fully modular, filters, substitutions, and entry limited to 9 lines
#ver 3  author: prenw499, Phillip Renwick, Q1 2016      prevent killing typescripts with clean if you are currently in a typescript
#ver 4  author: prenw499, Phillip Renwick, Q1 2016      uses external module list, absolves user of costs involved in upgrading to newer versions.
#ver 5  author: prenw499, Phillip Renwick, Q1 2017      SPECIALFLAGS, EXCLUDEFLAGS added
#ver 6  author: prenw499, Phillip Renwick, Q1 2017      (IN/EX)CLUSIVE delete is now optional
#ver 7  author: prenw499, Phillip Renwick, Q1 2017      now auto-clean's when switching from test builds to main builds and vice versa.
#ver 8  author: prenw499, Phillip Renwick, Q1 2017      fixed the 'test test all' and 'all all test' bug where it would build, clean, then try to link and always fail.
#ver 9  author: prenw499, Phillip Renwick, Q1 2017  	added auto git updating with make update
#ver10  author: prenw499, Phillip Renwick, Q1 2017	fixed the overly permissive delete exclusion.  No longer avoids test_file when asked not to delete file.

#build specific vars, must be changed for each new program.
include modules.mk   #must contain PROG, MODULES, TMODULES, DELOPTION, DELOPTIONFILES, FILES, DATAPATH, DATAFILES
-include submit.mk   #OPTIONAL - may contain submit instruction sets.
#to add in more modules use include.  include comes in two forms.  include is requisite, the file must exist or
#       it will fail.  -include is optional, make will try, bt not care if it cannot find it.

#below here should NOT require editing
#       this files does things which may catch you unaware and cause issues that may not be
#       imediately apparent.

#static vars, should not need editing.
PROTECTCODE=cpp h mk mm mt
PROTECTCODEFILES=makefile
PROTECTDEBUG=gcov gcno gcda
PROTECTPROG=$(PROG) test_$(PROG)
CPPFLAGS=-ansi -pedantic-errors -Wall -Wconversion -MD
SHELL=/bin/bash -O extglob -c #run make in bash, not sh, makes life much simpler.
empty=
space=$(empty) $(empty)
TOKEN=konsole
DELCMPTYPE=EXCLUSIVE
TESTLAST=
MAINLAST=

#ensures that typescripts are ALWAYS protected when in script session
ifeq ($(TOKEN),$(TERM))
PROTECTCODEFILES:=$(PROTECTCODEFILES) typescript
endif

ifeq ($(shell test -e ".mt" && echo -n yes),yes)
TESTLAST:=1
MAINLAST:=
endif

ifeq ($(shell test -e ".mm" && echo -n yes),yes)
MAINLAST:=1
TESTLAST:=
endif

#include directives for dependencies
#include will require files exist and fail make on error
#-include will silently pass errors, and allow make to continue on to attempt build
-include $(MODULES:.o=.d)
-include $(TMODULES:.o=.d)


#primary build rules
all:$(if $(TESTLAST),clean) 
all:CPPFLAGS:=$(filter-out $(EXCLUDEFLAGS),$(CPPFLAGS) $(SPECIALFLAGS))  #push all flags to CPPFLAGS for the implicit rule to utilise
all:$(MODULES)
all:mm
	g++ $(CPPFLAGS) $(MODULES) -o $(PROG)


#test build rules, redefines GTFLAGS and LDFLAGS to testing purposes.
#calls gcov on intended code when completed.
test:$(if $(MAINLAST),clean)  #TRUE if MAINLAST  is not empty, which means .mm exists, last build was all
test:GTFLG=-lgtest -lpthread -lgtest_main
test:LDFLG=-fprofile-arcs -ftest-coverage
test:CPPFLAGS:=$(filter-out $(EXCLUDEFLAGS),$(CPPFLAGS) $(GTFLG) $(LDFLG) $(SPECIALFLAGS))  #push all flags to CPPFLAGS for the implicit rule to utilise
test:$(TMODULES)
test:mt
	g++ $(CPPFLAGS) $(TMODULES) -g -o test_$(PROG)
	test_$(PROG)
	$(foreach var,$(filter $(MODULES:.o=.cpp), $(TMODULES:.o=.cpp)), gcov $(var) 2> /dev/null | grep -A 1 $(var);)

#used to determine if I last built test, or main, shoud clean first if building other else use built files as is
mt:
	@rm -f .mm	#building test, remove the .mm file that tracks main builds
	@echo "" > .mt  #building test, place the .mt file that tracks test builds

mm:
	@rm -f .mt	#building main, remove the .mt file that tracks test builds
	@echo "" > .mm  #building main, place the .mm file that tracks main builds

#clean which ignores protected CODE, FILES, DEBUG, PROGRrams, and directories.
#   or which only kills listed files.
clean:PROTECT=$(filter-out 'xxxxx', $(PROTECTCODE) $(PROTECTDEBUG))
clean:PROTECTEDCODEFILES:=$(filter-out 'xxxxx', $(PROTECTCODEFILES) $(PROTECTPROG) $(DELOPTFILES))
clean:PROTECTED=$(subst $(space),\|,$(PROTECT))
clean:PROTECTEDTEST:=$(PROTECTEDCODEFILES)
clean:PROTECTEDFILES=$(subst $(space),\|,$(PROTECTEDCODEFILES))
clean:	#if exclusive                               if typescript file exists                          if typescript not protected               mention it
	$(if $(filter $(DELCMPTYPE), $(DELTYPE)),$(if $(shell test -e "typescript" && echo -n yes),$(if $(filter typescript, $(PROTECTEDTEST)),@echo "clean: Typescript protected",$(call __TYPESCRIPT_WARN__))))
	$(if $(filter $(DELCMPTYPE), $(DELTYPE)),find . -maxdepth 1 ! -perm /a=x -type f ! -iregex '.*\($(PROTECTED)\)' ! -iregex './\($(PROTECTEDFILES)\)' | xargs rm -f;,rm -f $(DELOPTFILES))

#clean specific to removing unnecessary directories from current directory list
cleanDir:DIRS=$(shell find . -mindepth 1 -maxdepth 1 -type d)
cleanDir:
	$(if $(filter $(DELCMPTYPE), $(DELTYPE)),$(foreach var,$(DIRS), rm -r -i $(var);),@echo "Will not delete directories while performing inclusve delete")

#full clean of directory, if exclusive, this will also ask to kill child directories
cleanAll:PROTECT=$(filter-out 'xxxxx', $(PROTECTCODE))
cleanAll:PROTECTEDCODEFILES:=$(filter-out 'xxxxx', $(PROTECTCODEFILES) $(DELOPTFILES))
cleanAll:PROTECTEDTEST:=$(PROTECTEDCODEFILES)
cleanAll:PROTECTED=$(subst $(space),\|,$(PROTECT))
cleanAll:PROTECTEDFILES=$(subst $(space),\|,$(PROTECTEDCODEFILES))
cleanAll:DIRS=$(shell find . -mindepth 1 -maxdepth 1 -type d)
cleanAll:
	$(if $(filter $(DELCMPTYPE), $(DELTYPE)),$(if $(shell test -e "typescript" && echo -n yes),$(if $(filter typescript, $(PROTECTEDTEST)),@echo "clean: Typescript protected",$(call __TYPESCRIPT_WARN__))))
	$(if $(filter $(DELCMPTYPE), $(DELTYPE)),find . -maxdepth 1 -type f ! -iregex '.*\($(PROTECTED)\)' ! -iregex './\($(PROTECTEDFILES)\)' | xargs rm -f;,rm -f $(DELOPTFILES))
	$(if $(filter $(DELCMPTYPE), $(DELTYPE)),$(foreach var,$(DIRS), rm -r -i $(var);),@echo -n "")

#runs valgrind on program or test_program
v:
	valgrind --tool=memcheck --leak-check=full $(PROG)
vt:
	valgrind --tool=memcheck --leak-check=full test_$(PROG)

#copies datafiles from target directory
data:
	$(foreach var,$(DATAFILES),cp $(DATAPATH)$(var) .;)

update:
	@$(shell echo rm -f makefile)   #don't need to show, kill current because we asked for latest
	wget https://raw.githubusercontent.com/Athandreyal/Makefile/master/makefile

define __TYPESCRIPT_WARN__
	@echo ""
	@echo "================================"
	@echo "     typescript eliminated!"
	@echo "================================"
	@echo ""
endef

