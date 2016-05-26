LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := rm-wrapper
LOCAL_SRC_FILES := rm-wrapper.c
LOCAL_CFLAGS := -fPIE
LOCAL_LDFLAGS := -fPIE -pie -ldl -llog
LOCAL_SHARED_LIBRARIES := libc libdl liblog libm libstdc++
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := rm-wrapper-mrom
LOCAL_SRC_FILES := rm-wrapper.c
LOCAL_CFLAGS := -fPIE
LOCAL_LDFLAGS := -fPIE -pie -ldl -llog -Wl,--dynamic-linker=/sbin/touchscreen/linker
LOCAL_SHARED_LIBRARIES := libc libdl liblog libm libstdc++
include $(BUILD_EXECUTABLE)
