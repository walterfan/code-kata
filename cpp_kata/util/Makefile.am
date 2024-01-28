AUTOMAKE_OPTIONS = foreign

lib_LTLIBRARIES = libtinyutil.la
libtinyutil_la_SOURCES = TinyUtil.cpp  TinyTimer.cpp  TinyServer.cpp  TinyClient.cpp  TinyRio.cpp LuaUtil.cpp
libtinyutil_la_CPPFLAGS = -I/usr/include -I/usr/local/include
libtinyutil_la_LDFLAGS = -L/usr/lib -L/usr/local/lib
libtinyutil_la_LIBADD = -llua
#libtinyutil_la_DEPENDENCIES = all-local
