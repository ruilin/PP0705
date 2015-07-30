# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libpetgame
LOCAL_C_INCLUDES 		 		:= $(LOCAL_PATH)/engine 							\
									$(LOCAL_PATH)/engine/base	 								\
									$(LOCAL_PATH)/engine/base/base 							\
									$(LOCAL_PATH)/engine/base/engine 						\
									$(LOCAL_PATH)/engine/base/jni_main 					\
									$(LOCAL_PATH)/engine/java									\
									$(LOCAL_PATH)/engine/java/jni_lib							\
									$(LOCAL_PATH)/engine/render								\
									$(LOCAL_PATH)/engine/resource								\
									$(LOCAL_PATH)/engine/resource/res						\
									$(LOCAL_PATH)/engine/util										\
									$(LOCAL_PATH)/engine/util/data								\
									$(LOCAL_PATH)/engine/util/str_util							\
									$(LOCAL_PATH)/engine/util/algorithm						\
									$(LOCAL_PATH)/engine/util/algorithm/maths			\
									$(LOCAL_PATH)/engine/io										\
									$(LOCAL_PATH)/engine/io/file_op							\
									$(LOCAL_PATH)/engine/io/database						\
									$(LOCAL_PATH)/engine/runnable								\
									$(LOCAL_PATH)/engine/runnable/thread					\
									$(LOCAL_PATH)/engine/runnable/thread/gthread	\
									$(LOCAL_PATH)/engine/world									\
									$(LOCAL_PATH)/engine/ui										\
									$(LOCAL_PATH)/engine/ui/ui									\
									$(LOCAL_PATH)/engine/ui/view_include					\
									$(LOCAL_PATH)/engine/render								\
									$(LOCAL_PATH)/engine/render/render2d					\
									$(LOCAL_PATH)/engine/game									\
									$(LOCAL_PATH)/game												\
									$(LOCAL_PATH)/game/common								\
									$(LOCAL_PATH)/game/common/BaseCpp				\
									
################################################## ENGINE #####################################################											
LOCAL_SRC_FILES_ENGINE			:=	engine/base/jni_main.c							\
										engine/base/engine.c											\
										engine/base/sysinfo.c											\
										engine/base/debug.c											\
										engine/java/jni_lib.c												\
										engine/util/time_util.c											\
										engine/util/str_util.c												\
										engine/util/mem.c												\
										engine/util/linked_list.c											\
										engine/util/hashmap/crc32.c								\
										engine/util/hashmap/hash.c									\
										engine/util/data/endian.c										\
										engine/util/aes/aes256.c										\
										engine/util/data/rect.c											\
										engine/util/algorithm/maths.c								\
										engine/io/file_op.c												\
										engine/io/database.c											\
										engine/resource/image.c										\
										engine/resource/res.c											\
										engine/resource/png_reader.c								\
										engine/resource/pot.c											\
										engine/resource/map_data.c								\
										engine/resource/sprite_data.c								\
										engine/resource/script_data.c								\
										engine/render/render2d/renderer2d.c					\
										engine/render/render2d/graphic.c						\
										engine/render/render2d/canvas.c							\
										engine/render/render2d/matrix.c							\
										engine/render/render2d/text.c								\
										engine/runnable/runnable.c									\
										engine/runnable/thread/gthread.c						\
										engine/ui/ui.c														\
										engine/ui/toast.c													\
										engine/ui/view.c													\
										engine/ui/motion_event.c										\
#										engine/ui/button.c								\
							
LOCAL_CFLAGS    					:= -Werror
LOCAL_SRC_FILES 					:= 	$(LOCAL_SRC_FILES_ENGINE)				\
														gl_code.cpp											\
														game/World.cpp									\
														game/Sprite.cpp									\

LOCAL_LDLIBS    					:= -llog -landroid -lEGL -lGLESv2 -ljnigraphics -lz
LOCAL_WHOLE_STATIC_LIBRARIES 	:= png lua
include $(BUILD_SHARED_LIBRARY)

MY_LOCAL_PATH := $(LOCAL_PATH)
include $(LOCAL_PATH)/engine/ext/libpng/Android.mk
LOCAL_PATH := $(MY_LOCAL_PATH)
include $(LOCAL_PATH)/engine/ext/liblua/Android.mk
