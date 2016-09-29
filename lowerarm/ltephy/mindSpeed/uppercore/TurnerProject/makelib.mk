# Function to convert lower case letters to uppercase

transform = $(strip $(eval tmpstring := $3) $(foreach c,$(join $(addsuffix :,$1),$2),$(eval tmpstring := $(subst $(word 1,$(subst :, ,$c)),$(word 2,$(subst :, ,$c)),$(tmpstring))))$(tmpstring))

UPPER := A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
lower := a b c d e f g h i j k l m n o p q r s t u v w x y z

uppercase = $(call transform,$(lower),$(UPPER),$1)
lowercase = $(call transform,$(UPPER),$(lower),$1)
