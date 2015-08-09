/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.ryangame.pet.gl;

// Wrapper for native library

public class GL2JNILib {

     static {
         System.loadLibrary("petgame");
     }

    /**
     * @param width the current view width
     * @param height the current view height
     */
     public static native boolean init(Platform globalData, int screenWidth, int screenHeight,
    		 											int viewWidth, int viewHeight);
     public static native void create(Platform globalData);
     public static native Platform step(Platform globalData);
     public static native void destroy(Platform globalData);
     public static native Platform getGlobalData(Platform globalData);
}
