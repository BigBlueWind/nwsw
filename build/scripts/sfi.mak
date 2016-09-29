#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#File/Obj macros             
#-------------------------------------------------------------#
SFI_SRC_FILES= \
        sfi.c \
        sfi_gen.c \
	event_names.c

SFI_OBJ_FILES  := $(patsubst %.c, $(OUT_DIR)/%.o, $(SFI_SRC_FILES) )
	
SFICM_INC= $(IN_DIR)/sfi.h \
           $(IN_DIR)/../inc/pst_events.h 

RRM_BSM_INC= $(IN_DIR)/../inc/msg_ids.h \
	     $(IN_DIR)/../inc/bs_modules.h 

ALL_INC= $(SFICM_INC) $(RRM_BSM_INC) $(CM_INC)

BUILD_PREFIX            := "==> Building"
V                       := @ 
   
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#
#SFI ObjectFiles
$(OUT_DIR)/%.o: $(IN_DIR)/%.c
	@echo $(BUILD_PREFIX) $@
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS)  $< -o $@

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(LIB_DIR)/libsfi.a:$(SFI_OBJ_FILES) $(ALL_INC)
	ar -cr $(LIB_DIR)/libsfi.a $(SFI_OBJ_FILES)	

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(LIB_DIR)/libsfi.a $(SFI_OBJ_FILES)

