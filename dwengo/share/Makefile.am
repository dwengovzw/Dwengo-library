pkgdata_DATA = common.mk
dist_pkgdata_DATA = Makefile.proj Makefile.topprojdir
CLEANFILES = common.mk

all: common.mk
common.mk: FORCE
	@touch common.mk
	@echo "DWENGOLIB="$(pkglibexecdir) > common.mk
	@echo "DWENGOHEAD="$(pkgincludedir) >> common.mk
	@echo "PK2_BIN="$(PK2_BIN) >> common.mk
	@echo "PK2="$(PK2) >> common.mk
	@echo "MCC_INCLUDE="$(MCC_INCLUDE) >> common.mk
	@echo "MCC_LIB="$(MCC_LIB) >> common.mk
	@echo "MCC_L="$(MCC_L) >> common.mk
	@echo "MCC_C="$(MCC_C) >> common.mk
	@echo "MCC_LNK="$(MCC_LNK) >> common.mk
	@echo 'PROJECT = $${PROJ}' >> common.mk
	@echo "export DWENGOLIB DWENGOHEAD PK2_BIN PK2 MCC_INCLUDE MCC_LIB MCC_L MCC_C PROJECT" \
	                                                                  >> common.mk

FORCE:
	@$(RM) common.mk

.PHONY: all FORCE
