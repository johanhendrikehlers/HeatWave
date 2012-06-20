##############################################################################
##
## HeatLib, MiscLib & MiscTool MakeFile
## Johan Hendrik Ehlers <0200498@buckingham.ac.uk>
##
## $Date: 2007-08-06 08:39:38 $
## $Revision: 1.10 $
##
##############################################################################

MISCLIB = MiscLib.a
HEATLIB = HeatLib.a
TESTHW  = TestHW
SIMPLIB = SimpLib.a
TOOLAPP = MiscTool
IIIGET 	= IIIGet
IIISET	= IIISet

##############################################################################
# static settings
src_dir = ../../src/
inc_dir = ../../inc/ /usr/include/
src_suffix = .cpp
src_c_suffix = .c
misc_prefix = Misc
heat_prefix = Heat
test_prefix = Test
test_run = LastTestDate
simp_prefix = Simple
iii_prefix = III
lib_extern = /usr/lib/libjasper.a /usr/lib/libjpeg.a /usr/lib/libmhash.a
lib_cppunit = /usr/lib/libcppunit.a
dep_suffix = .d
obj_suffix = .o

##############################################################################
# dynamic settings (dont change things here!)
src_files = $(notdir $(wildcard $(src_dir)*$(src_suffix)))
src_c_files = $(notdir $(wildcard $(src_dir)*$(src_c_suffix)))
dep_files = $(src_files:$(src_suffix)=$(dep_suffix))
obj_files = $(src_files:$(src_suffix)=$(obj_suffix))
obj_heat = $(filter $(heat_prefix)%,$(obj_files))
obj_simp = $(filter $(simp_prefix)%,$(obj_files))
obj_misc_all = $(filter $(misc_prefix)%,$(obj_files))	
obj_misc = $(filter-out $(TOOLAPP)$(obj_suffix),$(obj_misc_all))
obj_test = $(filter $(test_prefix)%,$(obj_files))
obj_tool = $(TOOLAPP)$(obj_suffix)
obj_iiig = $(IIIGET)$(obj_suffix)
obj_iiis = $(IIISET)$(obj_suffix)
VPATH = $(inc_dir) $(src_dir)

##############################################################################
# compiler and linker
PP = g++
PPDEF = 
PPOUT = -o
PPCMP = -c
PPDBG = -g -D DEBUG
PPFLG = $(PPDBG) -pedantic -Wall $(patsubst %,-I%,$(VPATH)) $(DEFS)
PPDEP = -M
C = gcc
CDEF = 
COUT = -o
CCMP = -c
CDBG = -g -D DEBUG
CFLG = $(CDBG) -pedantic -Wall $(patsubst %,-I%,$(VPATH)) $(DEFS)
CDEP = -M
LN = ar
LNOPS = -rcvu

all : $(HEATLIB) $(MISCLIB) $(SIMPLIB) $(TOOLAPP) $(TESTHW) $(IIIGET) \
	  $(IIISET) $(test_run)

$(HEATLIB): $(obj_heat)
	$(LN) $(LNOPS) $@ $(obj_heat)

$(MISCLIB): $(HEATLIB) $(obj_misc) $(lib_extern)
	$(LN) $(LNOPS) $@ $(obj_misc)

$(SIMPLIB): $(obj_simp) $(HEATLIB)
	$(LN) $(LNOPS) $@ $(obj_simp) $(HEATLIB)

$(TOOLAPP): $(obj_tool) $(HEATLIB) $(MISCLIB) $(SIMPLIB) $(lib_extern)
	$(PP) $(PPOUT) $@ $(obj_tool) $(MISCLIB) $(HEATLIB) $(SIMPLIB) $(lib_extern)

$(TESTHW): $(HEATLIB) $(obj_test) $(lib_cppunit)
	$(PP) $(PPOUT) $@ $(obj_test) $(HEATLIB) $(lib_cppunit) 

$(test_run): $(HEATLIB) $(TESTHW)
	./$(TESTHW)
	date > $@

$(IIIGET): $(obj_iiig)
	$(C) $(COUT) $@ $(obj_iiig)

$(IIISET): $(obj_iiis)
	$(C) $(COUT) $@ $(obj_iiis)

%$(dep_suffix): %$(src_suffix)
	$(PP) $(PPFLG) $(PPDEP) $< > $@

%$(obj_suffix): %$(src_suffix)
	$(PP) $(PPCMP) $(PPFLG) $< $(PPOUT) $@

%$(obj_suffix): %$(src_c_suffix)
	$(C) $(CCMP) $(CFLG) $< $(COUT) $@

uml.dia :
	mkdir dia
	cp ../../inc/*.hpp ./dia/
	cp ../../src/*.cpp ./dia/
	cd dia ; cpp2dia.tclsh
	mv ./dia/output.dia uml.dia
	rm -Rf dia

-include $(dep_files)

.PHONY: clean
clean:
	-rm -f *.o *.d $(HEATLIB) $(MISCLIB) $(SIMPLIB) 
	-rm -f $(TOOLAPP) $(TESTHW) $(IIIGET) $(IIISET)
	-rm -Rf dia uml.dia
	-rm $(test_run)
