AUTOMAKE_OPTIONS = foreign
AM_CPPFLAGS=-I$(top_srcdir)/util -I/usr/local/include

LIBEVENT_DIR=/usr/local/Cellar/libevent/2.1.8

MOZJS_INC_DIR=/usr/local/include/mozjs-31

all-local:
#	cd $(GMOCK_DIR) &&  autoreconf -fvi && ./configure && $(MAKE) $(AM_MAKEFLAGS) 
#	cd $(PROTOBUF_DIR) && $(MAKE) $(AM_MAKEFLAGS) 
#	cd $(LUA_DIR) && $(MAKE) $(AM_MAKEFLAGS) posix
#	cd $(LUABIND_DIR) && $(MAKE) $(AM_MAKEFLAGS) all
	
check-local:
#	cd $(GMOCK_DIR) && $(MAKE) $(AM_MAKEFLAGS) test
#	cd $(PROTOBUF_DIR) && $(MAKE) $(AM_MAKEFLAGS) test
#	cd $(LUA_DIR) && $(MAKE) $(AM_MAKEFLAGS) test
#	cd $(LUABIND_DIR) && $(MAKE) $(AM_MAKEFLAGS) test
	
clean-local:
#	cd $(GMOCK_DIR) && $(MAKE) $(AM_MAKEFLAGS) clean
#	cd $(PROTOBUF_DIR) && $(MAKE) $(AM_MAKEFLAGS) clean
#	cd $(LUA_DIR) && $(MAKE) $(AM_MAKEFLAGS) clean
#	cd $(LUABIND_DIR) && $(MAKE) $(AM_MAKEFLAGS) clean


noinst_PROGRAMS = waltertest unittest 



waltertest_SOURCES = waltertest.cpp
waltertest_CPPFLAGS = -I$(top_srcdir)/util -I$(top_srcdir)/src -Wall -g -O0 --coverage
# -fprofile-arcs -ftest-coverage
waltertest_LDADD=../util/.libs/libtinyutil.a 
waltertest_LDFLAGS=-D_GNU_SOURCE --coverage 
#-lgcov
DEFS+=-D_GNU_SOURCE

unittest_SOURCES = UnitTest.cpp server_test.cpp lua_test.cpp TinyUtilTest.cpp
unittest_CPPFLAGS = -I$(GMOCK_DIR)/include -I$(GTEST_DIR)/include -I$(LUA_DIR)/src -I$(top_srcdir)/util
unittest_DEPENDENCIES = all-local
unittest_LDFLAGS = -L$(GMOCK_DIR)/lib -L$(GTEST_DIR)/lib -L$(PROTOBUF_DIR)/src -L$(LUA_DIR)/src -L$(top_srcdir)/util
unittest_LDADD = -lgmock -lgtest -llua -ltinyutil

