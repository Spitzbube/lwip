################################################################################
################################################################################
##
## Makefile -- project specific makefile to run build on sub directories
##
## (C) Copyright 2006-2010 by Fujitsu Microelectronics Europe GmbH
## (C) Copyright 2010-2012 by Fujitsu Semiconductor Europe GmbH
##
## Version: $Id: Makefile 9694 2012-08-15 06:08:17Z jmorha $
##
## Mandatory settings:
##
## o TOPDIR      = the toplevel directory (using slashes as path separator)
## o TARGET      = the machine qualifier to generate code for
## o DIRECTORIES = the list of sub directories to build
##
################################################################################
################################################################################

  TOPDIR = ../../..
  SUBDIR = subsystem/net/lwip

include $(TOPDIR)/env/make/Makefile.config

################################################################################
# project specific settings
################################################################################

  DIRECTORIES  = src

################################################################################
# define user targets
################################################################################

default: build

clean: $(DIRECTORIES:%=subdir-clean-%)

build: $(DIRECTORIES:%=subdir-build-%)

headers: $(DIRECTORIES:%=subdir-headers-%)

install: $(DIRECTORIES:%=subdir-install-%)

all: clean build install

################################################################################
# include internal definitions and rules
################################################################################

release: release-files $(DIRECTORIES:%=subdir-release-%)

doc: $(DIRECTORIES:%=subdir-doc-%)

include $(TOPDIR)/env/make/Makefile.rules

################################################################################
# local directory specific rules
################################################################################

subdir-clean-%:
	@$(NEWLINE)
	@$(ECHO) $(ECHOPREFIX) Submaking clean [$(SUBDIR)/$(@:subdir-clean-%=%)] for target [$(TARGET)]
	@$(SUBMAKE) -C $(@:subdir-clean-%=%) clean TARGET=$(TARGET)

subdir-depend-%:
	@$(NEWLINE)
	@$(ECHO) $(ECHOPREFIX) Submaking depend [$(SUBDIR)/$(@:subdir-depend-%=%)] for target [$(TARGET)]
	@$(SUBMAKE) -C $(@:subdir-depend-%=%) depend TARGET=$(TARGET)

subdir-headers-%:
	@$(NEWLINE)
	@$(ECHO) $(ECHOPREFIX) Submaking headers [$(SUBDIR)/$(@:subdir-headers-%=%)] for target [$(TARGET)]
	@$(SUBMAKE) -C $(@:subdir-headers-%=%) headers TARGET=$(TARGET)

subdir-build-%:
	@$(NEWLINE)
	@$(ECHO) $(ECHOPREFIX) Submaking build [$(SUBDIR)/$(@:subdir-build-%=%)] for target [$(TARGET)]
	@$(SUBMAKE) -C $(@:subdir-build-%=%) build TARGET=$(TARGET)

subdir-install-%:
	@$(NEWLINE)
	@$(ECHO) $(ECHOPREFIX) Submaking install [$(SUBDIR)/$(@:subdir-install-%=%)] for target [$(TARGET)]
	@$(SUBMAKE) -C $(@:subdir-install-%=%) install TARGET=$(TARGET)

subdir-release-%:
	@$(NEWLINE)
	@$(ECHO) $(ECHOPREFIX) Submaking release [$(SUBDIR)/$(@:subdir-release-%=%)] for target [$(TARGET)]
	@$(SUBMAKE) -C $(@:subdir-release-%=%) release TARGET=$(TARGET)

subdir-doc-%:
	@$(NEWLINE)
	@$(ECHO) $(ECHOPREFIX) Submaking doc [$(SUBDIR)/$(@:subdir-doc-%=%)] for target [$(TARGET)]
	@$(SUBMAKE) -C $(@:subdir-doc-%=%) doc TARGET=$(TARGET)

patch-zip : force
ifeq ($(HOST),cygwin32)
	@$(NEWLINE)
	@$(ECHO) $(ECHOPREFIX) Creating patch ZIP file [$(PATCH_ZIP)$(ZIP_EXT)]
	$(ATSIGN)$(DELETE) $(DELETEFLAGS) $(PATCH_ZIP)$(ZIP_EXT)
	$(ATSIGN)$(ZIP) $(ZIPFLAGS) $(ZIPFLAGS_USER) $(PATCH_ZIP) $(PATCH_FILES)
else
	@$(NEWLINE)
	@$(ECHO) $(ECHOPREFIX) Creating patch ZIP file not supported on host $(HOST)
endif
