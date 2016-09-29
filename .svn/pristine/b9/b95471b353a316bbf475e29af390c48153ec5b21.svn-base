#
# Define customer files here
#
SRC = \
	ENBMAC\\*.c \
	ENBMAC\\*.h \
        ENODEB_CCPU\\*.* \
	ENBMAC\\make* \
	.cproject \
	.project \
	*.c \
	*.h

#
# Define target path(s) here
#
TARGETPATH = $(TARGETDIR)/LteTestMac

.PHONY : copyfiles
copyfiles:
	-$(MKDIR) $(TARGETPATH)
	-$(CP) $(SRC) $(TARGETPATH)
