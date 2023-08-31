################################################################################
################################################################################
##
## Makefile -- project specific makefile to build an object library
##
## (C) Copyright 2006-2010 by Fujitsu Microelectronics Europe GmbH
## (C) Copyright 2010-2012 by Fujitsu Semiconductor Europe GmbH
##
## Version: $Id: Makefile 9695 2012-08-15 07:58:02Z jmorha $
##
## Mandatory settings:
##
## o TOPDIR    = the toplevel directory (using slashes as path separator)
## o TARGET    = the machine qualifier to generate code for
## o SOURCES   = the list of source files to compile
## o STARTUP   = the name of the (assembler) startup file to use
## o HEADERS   = the list of local header files to install
## o PROGRAM   = the name of the executable to generate (without extension)
## o LIBRARIES = the list of library files to be linked
##
################################################################################
################################################################################

  TOPDIR = ../../../..
  SUBDIR = subsystem/net/lwip/src

include $(TOPDIR)/env/make/Makefile.config

################################################################################
# project specific settings
################################################################################

  SOURCES += api/api_lib.c
  SOURCES += api/api_msg.c
#  SOURCES += api/err.c
  SOURCES += api/netbuf.c
#  SOURCES += api/netdb.c
#  SOURCES += api/netifapi.c
  SOURCES += api/sockets.c
  SOURCES += api/tcpip.c
  SOURCES += core/def.c
  SOURCES += core/dhcp.c
  SOURCES += core/dns.c
  SOURCES += core/init.c
  SOURCES += core/mem.c
  SOURCES += core/memp.c
  SOURCES += core/netif.c
  SOURCES += core/pbuf.c
  SOURCES += core/raw.c
  SOURCES += core/stats.c
#  SOURCES += core/sys.c
  SOURCES += core/tcp.c
  SOURCES += core/tcp_in.c
  SOURCES += core/tcp_out.c
  SOURCES += core/timers.c
  SOURCES += core/udp.c
#  SOURCES += core/ipv4/autoip.c
  SOURCES += core/ipv4/icmp.c
  SOURCES += core/ipv4/igmp.c
#  SOURCES += core/ipv4/inet.c
  SOURCES += core/ipv4/inet_chksum.c
  SOURCES += core/ipv4/ip.c
  SOURCES += core/ipv4/ip_addr.c
#  SOURCES += core/ipv4/ip_frag.c
# SOURCES += core/ipv6/icmp6.c
# SOURCES += core/ipv6/inet6.c
# SOURCES += core/ipv6/ip6.c
# SOURCES += core/ipv6/ip6_addr.c
#  SOURCES += core/snmp/asn1_dec.c
#  SOURCES += core/snmp/asn1_enc.c
#  SOURCES += core/snmp/mib2.c
#  SOURCES += core/snmp/mib_structs.c
#  SOURCES += core/snmp/msg_in.c
#  SOURCES += core/snmp/msg_out.c
  SOURCES += netif/etharp.c
  SOURCES += netif/ethif_fapi.c
  SOURCES += netif/ethif_asixusb.c
# SOURCES += netif/slipif.c
# SOURCES += netif/ppp/auth.c
# SOURCES += netif/ppp/chap.c
# SOURCES += netif/ppp/chpms.c
# SOURCES += netif/ppp/fsm.c
# SOURCES += netif/ppp/ipcp.c
# SOURCES += netif/ppp/lcp.c
# SOURCES += netif/ppp/magic.c
# SOURCES += netif/ppp/md5.c
# SOURCES += netif/ppp/pap.c
# SOURCES += netif/ppp/ppp.c
# SOURCES += netif/ppp/ppp_oe.c
# SOURCES += netif/ppp/randm.c
# SOURCES += netif/ppp/vj.c
  SOURCES += arch/sys_arch.c

  HEADERS += include/ipv4/lwip/autoip.h
  HEADERS += include/ipv4/lwip/icmp.h
  HEADERS += include/ipv4/lwip/igmp.h
  HEADERS += include/ipv4/lwip/inet.h
  HEADERS += include/ipv4/lwip/inet_chksum.h
  HEADERS += include/ipv4/lwip/ip.h
  HEADERS += include/ipv4/lwip/ip_addr.h
  HEADERS += include/ipv4/lwip/ip_frag.h
# HEADERS += include/ipv6/lwip/icmp.h
# HEADERS += include/ipv6/lwip/inet.h
# HEADERS += include/ipv6/lwip/ip.h
# HEADERS += include/ipv6/lwip/ip_addr.h
  HEADERS += include/lwip/api.h
  HEADERS += include/lwip/api_msg.h
  HEADERS += include/lwip/arch.h
  HEADERS += include/lwip/debug.h
  HEADERS += include/lwip/def.h
  HEADERS += include/lwip/dhcp.h
  HEADERS += include/lwip/dns.h
  HEADERS += include/lwip/err.h
  HEADERS += include/lwip/init.h
  HEADERS += include/lwip/lwipopts.h
  HEADERS += include/lwip/mem.h
  HEADERS += include/lwip/memp.h
  HEADERS += include/lwip/memp_std.h
  HEADERS += include/lwip/netbuf.h
  HEADERS += include/lwip/netdb.h
  HEADERS += include/lwip/netif.h
  HEADERS += include/lwip/netifapi.h
  HEADERS += include/lwip/opt.h
  HEADERS += include/lwip/pbuf.h
  HEADERS += include/lwip/raw.h
#  HEADERS += include/lwip/sio.h
  HEADERS += include/lwip/snmp.h
#  HEADERS += include/lwip/snmp_asn1.h
#  HEADERS += include/lwip/snmp_msg.h
#  HEADERS += include/lwip/snmp_structs.h
  HEADERS += include/lwip/sockets.h
  HEADERS += include/lwip/stats.h
  HEADERS += include/lwip/sys.h
  HEADERS += include/lwip/timers.h
  HEADERS += include/lwip/tcp.h
  HEADERS += include/lwip/tcp_impl.h
  HEADERS += include/lwip/tcpip.h
  HEADERS += include/lwip/udp.h

#  NETIF_HEADERS += include/netif/slipif.h
#  NETIF_HEADERS += include/netif/ppp_oe.h
  NETIF_HEADERS += include/netif/etharp.h
#  NETIF_HEADERS += include/netif/ethif_fapi.h

  ARCH_HEADERS += include/arch/sys_arch.h
#  ARCH_HEADERS += include/arch/perf.h
  ARCH_HEADERS += include/arch/cc.h

  LIBRARY       = liblwip
  HEADER_SUBDIR = lwip

ifeq ($(TARGET),arm11-gnuarm)
  CCFLAGS_USER += -Wno-strict-aliasing
  CCFLAGS_USER += -D__LINUX_ERRNO_EXTENSIONS__=1
endif

################################################################################
# special compilation settings
################################################################################

# additional include directories
  CCFLAGS_USER += -I$(INSTALL_INCLUDE_DIR)

  CCFLAGS_USER += --thumb
  
# lint flags
  NOMISRA = 1                   # completely deactivate lint for lwIP
  NODEBUG = 1 

################################################################################
# WiFi extension
################################################################################
-include Makefile.hedwifi

################################################################################
# define user targets
################################################################################

default: build

clean: clean-objects clean-library

build: headers build-objects build-library

headers: install-headers install-lwip-headers

install: build install-library

all: clean build install

################################################################################
# include internal definitions and rules
################################################################################

release: doc release-files

doc:

include $(TOPDIR)/env/make/Makefile.rules

install-lwip-headers:
	@$(NEWLINE)
	@$(ECHO) $(ECHOPREFIX) Installing lwip-specific headers [netif/*.h arch/*.h]
	@$(ECHO) $(ECHOPREFIX) "-->" [$(INSTALL_DIR)/$(DECODER_SUBDIR)/include/]
	$(ATSIGN)$(MKDIR) $(MKDIRFLAGS) $(INSTALL_DIR)/$(DECODER_SUBDIR)/include/$(HEADER_SUBDIR)/netif
	$(ATSIGN)$(MCOPY) $(MCOPYFLAGS) $(NETIF_HEADERS) $(INSTALL_DIR)/$(DECODER_SUBDIR)/include/$(HEADER_SUBDIR)/netif
	$(ATSIGN)$(MKDIR) $(MKDIRFLAGS) $(INSTALL_DIR)/$(DECODER_SUBDIR)/include/$(HEADER_SUBDIR)/arch
	$(ATSIGN)$(MCOPY) $(MCOPYFLAGS) $(ARCH_HEADERS) $(INSTALL_DIR)/$(DECODER_SUBDIR)/include/$(HEADER_SUBDIR)/arch

################################################################################
# include optional dependencies
################################################################################

-include $(DEPENDENCY_FILENAME)
