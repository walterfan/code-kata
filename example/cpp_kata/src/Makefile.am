AUTOMAKE_OPTIONS = foreign
AM_CPPFLAGS=-I$(top_srcdir)/util -I/usr/local/include


all-local:
#	cd $(LIBEVENT_DIR) && ./autogen.sh && ./configure && $(MAKE) $(AM_MAKEFLAGS)
#	cd $(PROTOBUF_DIR) && ./autogen.sh && ./configure && $(MAKE) $(AM_MAKEFLAGS) 
#	cd $(LUA_DIR) && $(MAKE) $(AM_MAKEFLAGS) posix
	
check-local:
#	cd $(LIBEVENT_DIR) && $(MAKE) $(AM_MAKEFLAGS) verify
#	cd $(PROTOBUF_DIR) && $(MAKE) $(AM_MAKEFLAGS) test
#	cd $(LUA_DIR) && $(MAKE) $(AM_MAKEFLAGS) test
	
clean-local:
#	cd $(LIBEVENT_DIR) && $(MAKE) $(AM_MAKEFLAGS) clean
#	cd $(PROTOBUF_DIR) && $(MAKE) $(AM_MAKEFLAGS) clean
#	cd $(LUA_DIR) && $(MAKE) $(AM_MAKEFLAGS) clean

noinst_PROGRAMS = hfrtc admin

hfrtc_SOURCES = hfrtc.cpp
hfrtc_CPPFLAGS = -I$(top_srcdir)/util
hfrtc_LDFLAGS = -L$(top_srcdir)/util 
hfrtc_LDADD = -ltinyutil -llua -levent 
hfrtc_DEPENDENCIES = all-local

admin_SOURCES = admin.cpp
admin_CPPFLAGS = -I$(top_srcdir)/util
admin_LDFLAGS = -L$(top_srcdir)/util 
admin_LDADD = -ltinyutil -llua -ldl
admin_DEPENDENCIES = all-local
