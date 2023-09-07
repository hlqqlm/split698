.PHONY : id_filename_table_dup_if_not_exist xcp

AM_CXXFLAGS = $(WARN_CXXFLAGS)

bin_PROGRAMS = split698


# $(includdir)的值由autoconf为我们自动生成，默认是'/urs/include' 或 '/usr/local/include'
# _CPPFLAGS指定预处理阶段头文件的查找目录(gcc -I)
# 如果是make distcheck，即用VPATH build，则动态生成的文件，是在builddir下，而不是src_dir下。因此，下面路径要指到buiddir
split698_CPPFLAGS = \
	-I$(includedir) \
	-I$(builddir)	\
	-I$(builddir)/qos	\
	-I$(builddir)/protocol	\
	-I$(builddir)/dlt698	\
	$(NULL)

# split698_LDFLAGS = $(top_builddir)/src/protocol/libprotocolib.a 
split698_LDFLAGS = $(top_srcdir)/src/protocol/libprotocolib.a 
EXTRA_DIST = \
			 protocol/libprotocolib.a 		\
			 protocol/libprotocolib.la 

#	protocol/qdo.cc protocol/qpart.cc protocol/qfill.cc	protocol/qfill_repository.cc			
split698_SOURCES = 	\
	main.cc 		\
	qos/fileid.cc qos/qcp.cc qos/qstr.cc qos/qtime.cc qos/time64.cc qos/qbit.cc qos/qtest.cc qos/qmem.cc qos/qarray.cc qos/qmemtable.cc qos/qtimeout.cc qos/qbuf.cc	\
	dlt698/qdlt698.cc dlt698/p2_base.cc dlt698/a_xdr.cc dlt698/p2_dar.cc dlt698/fcs.cc  \
	dlt698/p2_null.cc dlt698/p2_datatype_with_content.cc dlt698/p2_double_long_unsigned.cc dlt698/p2_array.cc dlt698/p2_struct.cc dlt698/p2_choice.cc dlt698/p2_double_long.cc dlt698/p2_oad.cc \
	dlt698/p2_a_result_normal.cc dlt698/p2_data_choice.cc dlt698/p2_follow_report.cc dlt698/p2_timetag.cc dlt698/p2_sequence_of_var_len.cc dlt698/p2_long_unsigned.cc dlt698/p2_long64.cc dlt698/p2_datetime.cc dlt698/p2_datetimes.cc dlt698/p2_nfix.cc dlt698/p2_nvar.cc dlt698/p2_octet_string.cc dlt698/p2_visible_string.cc dlt698/p2_tsa.cc dlt698/p2_rn.cc dlt698/p2_optional.cc dlt698/p2_enum.cc dlt698/p2_comdcb.cc dlt698/p2_trans_result.cc	dlt698/p2_omd_data.cc dlt698/p2_oad_data.cc dlt698/p2_omd_data_oad_unsigned.cc dlt698/p2_oad_data_oad_unsigned.cc dlt698/p2_omd_dar_data_optional.cc dlt698/p2_omd_dar_data_optional_aresultnormal.cc \
	dlt698/p2_rsd.cc dlt698/p2_selector1.cc \
	dlt698/p2_csd.cc dlt698/p2_road.cc dlt698/p2_rcsd.cc \
	dlt698/p2_addr.cc 	\
	dlt698/p2_error_response.cc 	\
	dlt698/p2_a_compact_result_record.cc dlt698/p2_sequence_of_a_record_row.cc dlt698/p2_a_record_row.cc	\
	dlt698/p2_link_apdu.cc dlt698/p2_link_apdu_choice.cc dlt698/p2_link_request.cc dlt698/p2_link_response.cc \
	dlt698/p2_connect_request.cc dlt698/p2_connect_mechanism_info_choice.cc dlt698/p2_signature_security.cc dlt698/p2_symmetry_security.cc \
	dlt698/p2_release_request.cc \
	dlt698/p2_get_request.cc dlt698/p2_get_request_normal.cc dlt698/p2_get_request_normal_list.cc \
	dlt698/p2_set_request_choice.cc dlt698/p2_set_request_normal.cc dlt698/p2_set_request_normal_list.cc dlt698/p2_set_then_get_request_normal_list.cc \
	dlt698/p2_action_request_choice.cc dlt698/p2_action_request_normal.cc dlt698/p2_action_request_normal_list.cc dlt698/p2_action_then_get_request_normal_list.cc \
	dlt698/p2_report_response_choice.cc dlt698/p2_report_response_record_list.cc \
	dlt698/p2_proxy_request_choice.cc dlt698/p2_proxy_request_trans_command_request.cc \
	dlt698/p2_compact_get_request.cc dlt698/p2_get_request_record.cc dlt698/p2_get_record.cc \
	dlt698/p2_compact_set_request.cc	\
	dlt698/p2_compact_proxy_request.cc dlt698/p2_compact_proxy_get_request_normal.cc dlt698/p2_compact_proxy_get_request_record.cc	dlt698/p2_compact_proxy_set_request_normal.cc dlt698/p2_compact_proxy_set_then_get_request.cc dlt698/p2_compact_proxy_action_request.cc dlt698/p2_compact_proxy_action_then_get_request.cc	\
	dlt698/p2_client_apdu.cc dlt698/p2_client_apdu_choice.cc  \
	dlt698/p2_connect_response.cc dlt698/p2_factory_version.cc dlt698/p2_connect_response_info.cc dlt698/p2_security_data.cc	\
	dlt698/p2_release_response.cc \
	dlt698/p2_release_notification.cc \
	dlt698/p2_get_response.cc dlt698/p2_get_response_normal.cc dlt698/p2_get_response_normal_list.cc dlt698/p2_get_result.cc \
	dlt698/p2_set_response.cc dlt698/p2_set_response_normal.cc \
	dlt698/p2_action_response.cc dlt698/p2_action_response_normal.cc dlt698/p2_action_response_normal_list.cc dlt698/p2_action_then_get_response_normal_list.cc \
	dlt698/p2_report_notification.cc	dlt698/p2_report_notification_list.cc	\
	dlt698/p2_proxy_response_choice.cc dlt698/p2_proxy_response_trans_command_response.cc \
	dlt698/p2_compact_get_response.cc dlt698/p2_compact_get_response_normal.cc dlt698/p2_compact_get_response_normal_list.cc dlt698/p2_compact_get_response_record.cc \
	dlt698/p2_compact_set_response.cc dlt698/p2_compact_set_response_normal.cc	\
	dlt698/p2_compact_proxy_response.cc dlt698/p2_compact_proxy_get_response_normal.cc dlt698/p2_compact_proxy_get_response_record.cc dlt698/p2_compact_proxy_set_response.cc	\
	dlt698/p2_server_apdu.cc dlt698/p2_server_apdu_choice.cc 	\
	dlt698/p2_process_security_apdu.cc dlt698/p2_security_apdu_choice.cc dlt698/p2_security_request.cc dlt698/p2_security_request_data_choice.cc dlt698/p2_security_request_verify_choice.cc dlt698/p2_security_response.cc dlt698/p2_security_response_data_choice.cc dlt698/p2_security_response_verify.cc dlt698/p2_sequence_of.cc \
	dlt698/p2_process.cc dlt698/p2_process_client_apdu.cc dlt698/p2_process_server_apdu.cc dlt698/p2_process_link_apdu.cc 	\
	dlt698/p2_link.cc 



# protocol/qdo.h protocol/qpart.h protocol/qfill.h	protocol/qfill_repository.h			
# id_filename_table_uniq.h 
nobase_include_HEADERS = \
	qos/qlibc.h qos/app.h qos/time64_config.h qos/time64_limits.h qos/fileid.h qos/qcp.h qos/qstr.h qos/qtime.h qos/time64.h qos/qbit.h qos/qtest.h qos/qmem.h qos/qarray.h qos/qmemtable.h qos/qtimeout.h qos/qbuf.h	\
	protocol/pcut.h protocol/pdo.h protocol/pfill.h protocol/pfill_repository.h 			\
	dlt698/p2_config.h dlt698/qdlt698.h dlt698/p2_base.h dlt698/a_xdr.h dlt698/p2_dar.h dlt698/fcs.h  \
	dlt698/p2_null.h dlt698/p2_datatype_with_content.h dlt698/p2_double_long_unsigned.h dlt698/p2_array.h dlt698/p2_struct.h dlt698/p2_choice.h dlt698/p2_double_long.h dlt698/p2_oad.h \
	dlt698/p2_a_result_normal.h dlt698/p2_data_choice.h dlt698/p2_follow_report.h dlt698/p2_timetag.h dlt698/p2_sequence_of_var_len.h dlt698/p2_long_unsigned.h dlt698/p2_long64.h dlt698/p2_datetime.h dlt698/p2_datetimes.h dlt698/p2_nfix.h dlt698/p2_nvar.h dlt698/p2_octet_string.h dlt698/p2_visible_string.h dlt698/p2_tsa.h dlt698/p2_rn.h dlt698/p2_optional.h dlt698/p2_enum.h	dlt698/p2_comdcb.h dlt698/p2_trans_result.h dlt698/p2_omd_data.h dlt698/p2_oad_data.h dlt698/p2_omd_data_oad_unsigned.h dlt698/p2_oad_data_oad_unsigned.h dlt698/p2_omd_dar_data_optional.h dlt698/p2_omd_dar_data_optional_aresultnormal.h \
	dlt698/p2_rsd.h dlt698/p2_selector1.h \
	dlt698/p2_csd.h dlt698/p2_road.h dlt698/p2_rcsd.h \
	dlt698/p2_addr.h 	\
	dlt698/p2_error_response.h 	\
	dlt698/p2_a_compact_result_record.h dlt698/p2_sequence_of_a_record_row.h dlt698/p2_a_record_row.h	\
	dlt698/p2_link_apdu.h dlt698/p2_link_apdu_choice.h dlt698/p2_link_request.h dlt698/p2_link_response.h \
	dlt698/p2_connect_request.h dlt698/p2_connect_mechanism_info_choice.h dlt698/p2_signature_security.h dlt698/p2_symmetry_security.h \
	dlt698/p2_release_request.h \
	dlt698/p2_get_request.h dlt698/p2_get_request_normal.h dlt698/p2_get_request_normal_list.h \
	dlt698/p2_set_request_choice.h dlt698/p2_set_request_normal.h dlt698/p2_set_request_normal_list.h dlt698/p2_set_then_get_request_normal_list.h \
	dlt698/p2_action_request_choice.h dlt698/p2_action_request_normal.h dlt698/p2_action_request_normal_list.h dlt698/p2_action_then_get_request_normal_list.h \
	dlt698/p2_report_response_choice.h dlt698/p2_report_response_record_list.h \
	dlt698/p2_proxy_request_choice.h dlt698/p2_proxy_request_trans_command_request.h \
	dlt698/p2_compact_get_request.h dlt698/p2_get_request_record.h dlt698/p2_get_record.h \
	dlt698/p2_compact_set_request.h	\
	dlt698/p2_compact_proxy_request.h dlt698/p2_compact_proxy_get_request_normal.h dlt698/p2_compact_proxy_get_request_record.h	dlt698/p2_compact_proxy_set_request_normal.h dlt698/p2_compact_proxy_set_then_get_request.h	dlt698/p2_compact_proxy_action_request.h dlt698/p2_compact_proxy_action_then_get_request.h	\
	dlt698/p2_client_apdu.h dlt698/p2_client_apdu_choice.h   \
	dlt698/p2_connect_response.h dlt698/p2_factory_version.h dlt698/p2_connect_response_info.h	dlt698/p2_security_data.h \
	dlt698/p2_release_response.h \
	dlt698/p2_release_notification.h \
	dlt698/p2_get_response.h dlt698/p2_get_response_normal.h dlt698/p2_get_response_normal_list.h dlt698/p2_get_result.h \
	dlt698/p2_set_response.h dlt698/p2_set_response_normal.h \
	dlt698/p2_action_response.h dlt698/p2_action_response_normal.h dlt698/p2_action_response_normal_list.h dlt698/p2_action_then_get_response_normal_list.h \
	dlt698/p2_report_notification.h	dlt698/p2_report_notification_list.h	\
	dlt698/p2_proxy_response_choice.h dlt698/p2_proxy_response_trans_command_response.h \
	dlt698/p2_compact_get_response.h dlt698/p2_compact_get_response_normal.h dlt698/p2_compact_get_response_normal_list.h dlt698/p2_compact_get_response_record.h \
	dlt698/p2_compact_set_response.h dlt698/p2_compact_set_response_normal.h	\
	dlt698/p2_compact_proxy_response.h dlt698/p2_compact_proxy_get_response_normal.h dlt698/p2_compact_proxy_get_response_record.h dlt698/p2_compact_proxy_set_response.h	\
	dlt698/p2_server_apdu.h dlt698/p2_server_apdu_choice.h 	\
	dlt698/p2_process_security_apdu.h dlt698/p2_security_apdu_choice.h dlt698/p2_security_request.h dlt698/p2_security_request_data_choice.h dlt698/p2_security_request_verify_choice.h dlt698/p2_security_response.h dlt698/p2_security_response_data_choice.h dlt698/p2_security_response_verify.h dlt698/p2_sequence_of.h \
	dlt698/p2_process.h dlt698/p2_process_client_apdu.h dlt698/p2_process_server_apdu.h dlt698/p2_process_link_apdu.h 	\
	dlt698/p2_link.h 


####################################################################
# Below code creates *.xcp.h & id_filename_table_uniq.h
####################################################################
# ref: https://www.gnu.org/software/automake/manual/html_node/Program-Variables.html
# When a file to be included is generated during the build and not part of a distribution tarball, its location is under $(builddir), not under $(srcdir). This matters especially for packages that use header files placed in sub-directories and want to allow builds outside the source tree (see Parallel Build Trees (a.k.a. VPATH Builds)).


#XCP_SOURCE_PATH = . qos protocol dlt698
#XCP_SOURCE := $(foreach dir, $(XCP_SOURCE_PATH), $(wildcard $(dir)/*.c) $(wildcard $(dir)/*.cpp) $(wildcard $(dir)/*.cc) )
# XCP_SOURCE = $(filter %.c %.cc %cpp,$(split698_SOURCES)) 
XCP_SOURCE_SRCDIR = $(filter %.c %.cc %cpp,$(split698_SOURCES)) 
#XCP_SOURCE_BASENAME = $(notdir $(XCP_SOURCE_SRCDIR))
# XCP_SOURCE = $(subst $(top_srcdir),$(buildir),$(XCP_SOURCE_SRCDIR))
XCP_SOURCE = $(addprefix $(top_builddir)/src/, $(XCP_SOURCE_SRCDIR))
# XCP_SOURCE = $(patsubst %,$(builddir)/%,$(XCP_SOURCE_BASENAME))

XCP_HEADER := $(patsubst %.cc,%.xcp.h,$(XCP_SOURCE))      # name it as xcp.h, so NOT interrupt tab automatic complement for .cc & .h
FNI_HEADER := $(patsubst %.cc,%.fni,$(XCP_SOURCE)) 		# fni = file name item

FILENAME_HASH_CALC = $(shell echo -n 0x; echo -n $(word 1, $(shell tmpfile=$(shell mktemp); echo -n $(1) > $$tmpfile;  cksfv -c $$tmpfile | tail -1 | awk '{print $$2}';  rm -f $$tmpfile &> /dev/null)))


# *.fni *.xcp.h create by different targets, so they are decoupling
%.xcp.h : %.cc
	@echo -e "#define this_file_id $(call FILENAME_HASH_CALC,$<)\n//FILEID_REGISTER();" > $@
xcp: $(XCP_HEADER)


%.fni : %.cc
	@echo "{ $(call FILENAME_HASH_CALC,$<), \"$<\" }, " > $@


# 如果文件不存在，就删除所有xxx.xcp.h，重新生成文件
#id_filename_table_dup_if_not_exist:
#	@test -s id_filename_table_dup.h || { echo "id_filename_table_dup.h is not exist, rm all xxx.xcp.h, and recreate it."; $(RM-F) $(XCP_HEADER); }

id_filename_table_uniq.h: id_filename_table_sort.h
	uniq $< > $@

# 将从属库的id_filename_table_uniq.h插入进来
# cat ../libctlcomm/id_filename_table_uniq.h ../libctl698/id_filename_table_uniq.h >> id_filename_table_dup.h
id_filename_table_sort.h: id_filename_table_dup.h 
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
	@echo XCP_HEADER = $(XCP_HEADER)
	@echo $(includedir)
	@echo XCP_SOURCE_SRCDIR = $(XCP_SOURCE_SRCDIR)
	@echo top_builddir = $(top_builddir)
	@echo XCP_SOURCE = $(XCP_SOURCE)

