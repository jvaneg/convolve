#  I'll be brief, as an IA if you need a bit more information
#
#  HOW TO USE:
#	(*) make dependancy is in use, it will figure things out 
#		for you as long as naming is consistent
#	(*) gcov is applied to any module listed in both MODULES 
#		and TMODULES
#	(*) clean, cleanDir, and cleanAll are using an (IN/EX)CLUSIVE delete.
#	        - rm file deletes file.  this is inclusive because 
#		  you "included" the list of file(s) to kill
#		
#		  An exclusive delete names the files you may not delete.
#		  ALL others it finds are fair game and WILL be deleted.
#		  
#		  BE VERY CAREFUL CALLING CLEANALL FROM ~ with exclusive delete!
#		  It will happily clean up, and things will likely be cleaner than you expected!
#
#  the variable lists are:
#  PROG: the name of the program.  The test command will asseble 
#        a program of a similar name, starting with test_, so if
#        PROG=paint, then you would get a program named paint from
#        make all, and a program named test_paint from make test
#
#  MODULES:  these are the modules that comprise your main program.  
#            If your program needs them to run, they go here.
#
#  TMODULES:  these are the modules that comprise your test program. 
#	      GTEST flags are automatically added to the build for these 
#	      modules, but it is not necessary to be using gtest.
#
#  DELTYPE:  INCLUSIVE or EXCLUSIVE, will be EXCLUSIVE by default for all other strings.
#
#  DELOPTFILES:  INCLUSIVE:  only the files listed here will be used for the delete, no files are specified by default
#		 EXCLUSIVE:  The files listed here will be protected, along with the following defaults:
#		             clean:   *.cpp, *.h, *. mk, makefile, *.gcov, *.gcno, *.gcda, $(prog), $(test_prog)
#		             cleanAll:*.cpp, *.h, *. mk, makefile
#		             typescripts are automatically protected while in script sessions
#
#  DATAPATH:  The path to the assignment directory where the data files are located
#
#  DATAFLES:  The names of the files that are needed from the assignment directory
#
#  SPECIALFLAGS:  For 1633 students, Likely useful only for special command-line #defines.  I had 'other' needs as well.
#			Ex: SPECIALFLAGS=-Ddebug
#
#			#ifdef debug
#			#define DEBUG cout << __FILE__ << " " << __LINE__ << " "
#			#else
#			#define DEBUG if(false) cout
#			#endif
#
#			DEBUG << "debug text";
#
#  EXCLUDEFLAGS:  1633 STUDENTS SHOULD PROBABLY LEAVE THIS ALONE.  The only purpose is to disable default flags.


#program name
PROG=convolve
#main module list
MODULES=convolve.o
#test module list
TMODULES=
#delete method, (IN/EX)CLUSIVE
DELTYPE=INCLUSIVE
#files to be (in/ex)cluded from the delete commands, in/ex depends on line above
DELOPTFILES=$(PROG) $(MODULES) *.d
#edit DATAPATH or DATAFILES only if project relies on files containing input data.
DATAPATH=/users/library/csis/comp1633/assignments/a1/
DATAFILES=
#special compile flags
SPECIALFLAGS=
#disabled compile flags
EXCLUDEFLAGS=
