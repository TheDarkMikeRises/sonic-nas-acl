ACLOCAL_AMFLAGS=-I m4

bin_SCRIPTS = scripts/bin/*

cfgdir = $(sysconfdir)/sonic
cfg_SCRIPTS = scripts/acl/*.xml

pyutilsdir=$(libdir)/sonic
pyutils_SCRIPTS = scripts/lib/python/*.py

include_HEADERS=sonic/nas_acl_filter.h sonic/nas_acl_entry.h sonic/nas_acl_log.h sonic/nas_acl_common.h sonic/nas_acl_switch_list.h sonic/nas_acl_cps.h sonic/nas_acl_cps_key.h sonic/nas_acl_action.h sonic/nas_acl_utl.h sonic/nas_acl_table.h sonic/nas_acl_counter.h sonic/nas_acl_switch.h sonic/nas_acl_init.h
lib_LTLIBRARIES=libsonic_nas_acl.la

libsonic_nas_acl_la_SOURCES=src/nas_acl_init.cpp src/nas_acl_table.cpp src/nas_acl_cps_counter.cpp src/nas_acl_counter.cpp src/nas_acl_action.cpp src/nas_acl_cps_stats.cpp src/nas_acl_cps_action_map.cpp src/nas_acl_entry.cpp src/nas_acl_cps_filter.cpp src/nas_acl_cps_utils.cpp src/nas_acl_filter.cpp src/nas_acl_switch.cpp src/nas_acl_cps_action.cpp src/nas_acl_cps_table.cpp src/nas_acl_cps_filter_map.cpp src/nas_acl_switch_list.cpp src/nas_acl_utl.cpp src/nas_acl_cps_entry.cpp src/nas_acl_cps.cpp

libsonic_nas_acl_la_CPPFLAGS= -D_FILE_OFFSET_BITS=64 -I$(top_srcdir)/sonic -I$(includedir)/sonic -I$(top_srcdir)/inc
libsonic_nas_acl_la_CXXFLAGS=-std=c++11
libsonic_nas_acl_la_LDFLAGS=-shared -version-info 1:1:0
libsonic_nas_acl_la_LIBADD=-lsonic_common -lsonic_nas_common -lsonic_nas_ndi -lsonic_object_library -lsonic_logging

systemdconfdir=/lib/systemd/system
systemdconf_DATA = scripts/init/*.service
