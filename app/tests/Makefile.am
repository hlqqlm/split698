.PHONY : xcp

AM_CPPFLAGS = -isystem $(top_srcdir)/lib/gtest/include -I$(top_srcdir)/src -I$(top_builddir)/src -I$(top_builddir)/src/qos -I$(top_builddir)/src/protocol -I$(top_builddir)/src/dlt698  $(GTEST_CPPFLAGS)
AM_CXXFLAGS = $(GTEST_CXXFLAGS)
AM_LDFLAGS = $(GTEST_LDFLAGS) $(GTEST_LIBS) \
	     $(top_builddir)/lib/gtest/lib/libgtest_main.la \
	     $(top_builddir)/lib/gtest/lib/libgtest.la

# TESTS -- Programs run automatically by "make check"
# check_PROGRAMS -- Programs built by "make check" but not necessarily run
TESTS = 
check_PROGRAMS = 

# TESTS += test1 
# check_PROGRAMS += test1 
# test1_SOURCES = func1_test.cc $(top_srcdir)/src/func.cc
# test1_LDADD = $(top_builddir)/src/func.o


# TESTS += test2
# check_PROGRAMS += test2
# test2_SOURCES = func2_test.cc $(top_srcdir)/src/func.cc
# test2_LDADD = $(top_builddir)/src/func.o

TESTS += qstr_ut
check_PROGRAMS += qstr_ut
# You can NOT use makefile functions to generate filename with path, such as $(addprefix) $(patsubst)
qstr_ut_SOURCES = qstr_ut.cc $(top_srcdir)/src/qos/qstr.cc $(top_srcdir)/src/qos/qcp.cc $(top_srcdir)/src/qos/fileid.cc
qstr_ut_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/src/qos $(NULL)


TESTS += qtime_ut
check_PROGRAMS += qtime_ut
# You can NOT use makefile functions to generate filename with path, such as $(addprefix) $(patsubst)
qtime_ut_SOURCES = qtime_ut.cc $(top_srcdir)/src/qos/qcp.cc $(top_srcdir)/src/qos/fileid.cc $(top_srcdir)/src/qos/qtime.cc $(top_srcdir)/src/qos/time64.cc 
qtime_ut_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/src/qos $(NULL)


TESTS += p2_addr_ut
check_PROGRAMS += p2_addr_ut
# p2_addr_ut_SOURCES = p2_addr_ut.cc $(top_srcdir)/src/dlt698/p2_addr.cc $(top_srcdir)/src/protocol/qdo.cc $(top_srcdir)/src/protocol/qpart.cc  $(top_srcdir)/src/protocol/qfill.cc $(top_srcdir)/src/qos/qstr.cc $(top_srcdir)/src/qos/qcp.cc $(top_srcdir)/src/qos/fileid.cc $(top_srcdir)/src/qos/qarray.cc $(top_srcdir)/src/qos/qmem.cc 
p2_addr_ut_SOURCES = p2_addr_ut.cc $(top_srcdir)/src/dlt698/p2_addr.cc  $(top_srcdir)/src/qos/qstr.cc $(top_srcdir)/src/qos/qcp.cc $(top_srcdir)/src/qos/fileid.cc $(top_srcdir)/src/qos/qarray.cc $(top_srcdir)/src/qos/qmem.cc 
p2_addr_ut_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/src/qos -I$(top_srcdir)/src/protocol -I$(top_srcdir)/src/dlt698  $(NULL)
p2_addr_ut_LDFLAGS = $(AM_LDFLAGS) $(top_srcdir)/src/protocol/libprotocolib.a 


TESTS += p2_double_long_ut
check_PROGRAMS += p2_double_long_ut
# p2_double_long_ut_SOURCES = p2_double_long_ut.cc $(top_srcdir)/src/dlt698/p2_double_long.cc $(top_srcdir)/src/dlt698/p2_datatype_with_content.cc $(top_srcdir)/src/protocol/qdo.cc $(top_srcdir)/src/protocol/qpart.cc  $(top_srcdir)/src/protocol/qfill.cc $(top_srcdir)/src/qos/qtest.cc $(top_srcdir)/src/qos/qstr.cc $(top_srcdir)/src/qos/qcp.cc $(top_srcdir)/src/qos/fileid.cc $(top_srcdir)/src/qos/qarray.cc $(top_srcdir)/src/qos/qmem.cc 
p2_double_long_ut_SOURCES = p2_double_long_ut.cc $(top_srcdir)/src/dlt698/p2_double_long.cc $(top_srcdir)/src/dlt698/p2_datatype_with_content.cc $(top_srcdir)/src/qos/qtest.cc $(top_srcdir)/src/qos/qstr.cc $(top_srcdir)/src/qos/qcp.cc $(top_srcdir)/src/qos/fileid.cc $(top_srcdir)/src/qos/qarray.cc $(top_srcdir)/src/qos/qmem.cc 
p2_double_long_ut_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/src/qos -I$(top_srcdir)/src/protocol -I$(top_srcdir)/src/dlt698  $(NULL)
p2_double_long_ut_LDFLAGS = $(AM_LDFLAGS) $(top_srcdir)/src/protocol/libprotocolib.a 



TESTS += p2_datetime_ut
check_PROGRAMS += p2_datetime_ut
#p2_datetime_ut_SOURCES = p2_datetime_ut.cc $(top_srcdir)/src/dlt698/p2_datetime.cc \
#	$(top_srcdir)/src/dlt698/p2_datatype_with_content.cc $(top_srcdir)/src/dlt698/qdlt698.cc $(top_srcdir)/src/dlt698/a_xdr.cc \
#	$(top_srcdir)/src/protocol/qdo.cc $(top_srcdir)/src/protocol/qpart.cc  $(top_srcdir)/src/protocol/qfill.cc 	\
#	$(top_srcdir)/src/qos/qtest.cc $(top_srcdir)/src/qos/qstr.cc $(top_srcdir)/src/qos/qcp.cc $(top_srcdir)/src/qos/fileid.cc $(top_srcdir)/src/qos/qarray.cc $(top_srcdir)/src/qos/qmem.cc 
p2_datetime_ut_SOURCES = p2_datetime_ut.cc $(top_srcdir)/src/dlt698/p2_datetime.cc \
	$(top_srcdir)/src/dlt698/p2_datatype_with_content.cc $(top_srcdir)/src/dlt698/qdlt698.cc $(top_srcdir)/src/dlt698/a_xdr.cc \
	$(top_srcdir)/src/qos/qtest.cc $(top_srcdir)/src/qos/qstr.cc $(top_srcdir)/src/qos/qcp.cc $(top_srcdir)/src/qos/fileid.cc $(top_srcdir)/src/qos/qarray.cc $(top_srcdir)/src/qos/qmem.cc 
p2_datetime_ut_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/src/qos -I$(top_srcdir)/src/protocol -I$(top_srcdir)/src/dlt698  $(NULL)
p2_datetime_ut_LDFLAGS = $(AM_LDFLAGS) $(top_srcdir)/src/protocol/libprotocolib.a 


TESTS += p2_octet_string_ut
check_PROGRAMS += p2_octet_string_ut
# $(top_srcdir)/src/protocol/qdo.cc $(top_srcdir)/src/protocol/qpart.cc  $(top_srcdir)/src/protocol/qfill.cc 	
p2_octet_string_ut_SOURCES = p2_octet_string_ut.cc $(top_srcdir)/src/dlt698/p2_octet_string.cc \
	$(top_srcdir)/src/dlt698/p2_datatype_with_content.cc $(top_srcdir)/src/dlt698/qdlt698.cc $(top_srcdir)/src/dlt698/a_xdr.cc \
	$(top_srcdir)/src/qos/qtest.cc $(top_srcdir)/src/qos/qstr.cc $(top_srcdir)/src/qos/qcp.cc $(top_srcdir)/src/qos/fileid.cc $(top_srcdir)/src/qos/qarray.cc $(top_srcdir)/src/qos/qmem.cc 
p2_octet_string_ut_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/src/qos -I$(top_srcdir)/src/protocol -I$(top_srcdir)/src/dlt698  $(NULL)
p2_octet_string_ut_LDFLAGS = $(AM_LDFLAGS) $(top_srcdir)/src/protocol/libprotocolib.a 




####################################################################
# Below code creates *.xcp.h & id_filename_table_uniq.h
####################################################################
#ref： https://www.gnu.org/software/automake/manual/html_node/Program-Variables.html
# https://www.gnu.org/software/automake/manual/html_node/Program-Variables.html
# 如果文件是VPATH build的，则动态生成的头文件，是在builddir，而不是在srcdir
# 所以，有两个重点：
# 1. 动态生成的头文件，路径是在$(top_builddir)/tests下，而文件名是从$(top_srcdir)/tests下获取的，因此，有个转换。
#    如果你用固定文件名列表，则不需要上面的转换了。
# 2. 头文件包含路径，要包括-I$(top_buiddir)/tests路径。

# XCP_SOURCE_PATH = . $(top_srcdir)/src/qos $(top_srcdir)/src/protocol $(top_srcdir)/src/dlt698
# XCP_SOURCE_PATH = . $(top_srcdir)/src/qos $(top_srcdir)/src/protocol $(top_srcdir)/src/dlt698
# XCP_SOURCE_PATH = . $(top_srcdir)/src/qos $(top_srcdir)/src/protocol $(top_srcdir)/src/dlt698
#XCP_SOURCE_PATH = $(top_srcdir)/tests
#XCP_SOURCE := $(foreach dir, $(XCP_SOURCE_PATH), $(wildcard $(dir)/*.c) $(wildcard $(dir)/*.cpp) $(wildcard $(dir)/*.cc))
#XCP_SOURCE = $(filter %.c %.cc %cpp,$(split698_SOURCES)) 
# XCP_SOURCE_PATH = $(top_srcdir)/tests
#XCP_SOURCE_PATH = $(top_srcdir)/tests $(top_srcdir)/src $(top_srcdir)/src/dlt698 $(top_srcdir)/src/qos $(top_srcdir)/src/protocol
XCP_SOURCE_PATH = $(top_srcdir)/tests 
#XCP_SOURCE_PATH = qstr_ut.cc p2_addr_ut.cc 
XCP_SOURCE_SRCDIR := $(foreach dir, $(XCP_SOURCE_PATH), $(wildcard $(dir)/*.c) $(wildcard $(dir)/*.cpp) $(wildcard $(dir)/*.cc))
# XCP_SOURCE_BASENAME = $(notdir $(XCP_SOURCE_SRCDIR))
#XCP_SOURCE_BASENAME = $(subst $(top_srcdir)/tests/,,$(XCP_SOURCE_SRCDIR))
# XCP_SOURCE = $(patsubst %,$(builddir)/%,$(XCP_SOURCE_BASENAME))
#XCP_SOURCE = $(addprefix $(top_builddir)/tests/, $(XCP_SOURCE_SRCDIR))
#XCP_SOURCE = $(subst $(top_srcdir),$(buildir),$(XCP_SOURCE_SRCDIR))
#XCP_SOURCE = $(XCP_SOURCE_SRCDIR)
XCP_SOURCE = $(subst $(top_srcdir)/tests/,$(top_builddir)/tests/,$(XCP_SOURCE_SRCDIR))

XCP_HEADER := $(patsubst %.cc,%.xcp.h,$(XCP_SOURCE))    # name it as xcp.h, so NOT interrupt tab automatic complement for .cc & .h
FNI_HEADER := $(patsubst %.cc,%.fni,$(XCP_SOURCE)) 		# fni = file name item

FILENAME_HASH_CALC = $(shell echo -n 0x; echo -n $(word 1, $(shell tmpfile=$(shell mktemp); echo -n $(1) > $$tmpfile;  cksfv -c $$tmpfile | tail -1 | awk '{print $$2}';  rm -f $$tmpfile &> /dev/null)))


# *.fni *.xcp.h create by different targets, so they are decoupling
%.xcp.h : %.cc
	@echo -e "#define this_file_id $(call FILENAME_HASH_CALC,$<)\n//FILEID_REGISTER();" > $@
xcp: $(XCP_HEADER)


%.fni : %.cc
	@echo $(FNI_HEADER)
	@echo "{ $(call FILENAME_HASH_CALC,$<), \"$<\" }, " > $@


# 如果文件不存在，就删除所有xxx.xcp.h，重新生成文件
#id_filename_table_dup_if_not_exist:
#	@test -s id_filename_table_dup.h || { echo "id_filename_table_dup.h is not exist, rm all xxx.xcp.h, and recreate it."; $(RM-F) $(XCP_HEADER); }

id_filename_table_uniq.h: id_filename_table_sort.h
	uniq $< > $@

../src/id_filename_table_uniq.h:
	cd ../src && $(MAKE) id_filename_table_uniq.h

# 将从属库的id_filename_table_uniq.h插入进来
# cat ../libctlcomm/id_filename_table_uniq.h ../libctl698/id_filename_table_uniq.h >> id_filename_table_dup.h
id_filename_table_sort.h: id_filename_table_dup.h ../src/id_filename_table_uniq.h
	cat ../src/id_filename_table_uniq.h >> id_filename_table_dup.h
	sort id_filename_table_dup.h > $@

id_filename_table_dup.h: $(FNI_HEADER)
	cat $^ >> $@
	
# 生成下面文件，在make all之前
BUILT_SOURCES = id_filename_table_uniq.h $(XCP_HEADER)
# make clean时，清除下面文件
CLEANFILES = id_filename_table_dup.h id_filename_table_sort.h id_filename_table_uniq.h
CLEANFILES += $(FNI_HEADER)
CLEANFILES += $(XCP_HEADER)



debug:
	@echo $(AM_CPPFLAGS)
	@echo top_srcdir = $(top_srcdir)
	@echo top_builddir = $(top_builddir)
	@echo XCP_SOURCE_SRCDIR = $(XCP_SOURCE_SRCDIR)
	@echo XCP_SOURCE_BASENAME = $(XCP_SOURCE_BASENAME)
	@echo XCP_SOURCE = $(XCP_SOURCE)
	@echo builddir = $(builddir)
	@echo XCP_HEADER = $(XCP_HEADER)
	@echo FNI_HEADER = $(FNI_HEADER)

repeat: $(check_PROGRAMS)
	for sub in $(check_PROGRAMS); \
	do \
		echo "run $$sub"; \
		./$$sub --gtest_repeat=1000;	\
	done


