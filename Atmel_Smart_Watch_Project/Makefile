CUR_DIR := $(subst C:,/c,${PWD})
SRC_DIR := ${CUR_DIR}/Smartwatch/Smartwatch/src

GCC_FLAGS := \
-Wno-all \
-Wno-int-to-pointer-cast \
-Wno-discarded-qualifiers \
-Wno-int-conversion \
-I${SRC_DIR} \
-I${SRC_DIR}/smartwatch/ \
-I${SRC_DIR}/smartwatch/display_driver \
-I${SRC_DIR}/smartwatch/display_driver/displays/default \
-I${SRC_DIR}/smartwatch/display_driver/GFXfont \
-I${SRC_DIR}/smartwatch/display_driver/GFXfont/Fonts \
-I${SRC_DIR}/smartwatch/display_driver/grapher \

FILE_LIST := \
${CUR_DIR}/console_test_main.c \
${SRC_DIR}/smartwatch/display_manager.c \
${SRC_DIR}/smartwatch/display_driver/display_driver.c \
${SRC_DIR}/smartwatch/display_driver/displays/default/console_display.c \
${SRC_DIR}/smartwatch/display_driver/GFXfont/gfxfont.h \
${SRC_DIR}/smartwatch/display_driver/grapher/grapher.c \
${SRC_DIR}/smartwatch/display_driver/grapher/util.c \

all:
	cd ${SRC_DIR} && \
	gcc ${GCC_FLAGS} ${FILE_LIST} -o ${CUR_DIR}/console_test.exe \
	-D CONSOLE_VERSION=1 \
	-D DISP_SCREEN=DISP_CONSOLE \
	-D CONSOLE_DISPLAY_WIDTH=128 \
	-D CONSOLE_DISPLAY_HEIGHT=128

clean:
	rm console_test.exe
