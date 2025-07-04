/*
 * Copyright (C) 2025 Kevin Buzeau
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../logs/log.h"
#include "jni.hpp"

using namespace smartautoclicker;

std::unique_ptr<cv::Mat> loadMatFromRGBA8888Bitmap(JNIEnv *env, jobject bitmap) {
    try {
        AndroidBitmapInfo info;
        void *pixels = nullptr;

        CV_Assert(AndroidBitmap_getInfo(env, bitmap, &info) >= 0);
        CV_Assert(info.format == ANDROID_BITMAP_FORMAT_RGBA_8888);
        CV_Assert(AndroidBitmap_lockPixels(env, bitmap, &pixels) >= 0);

        return std::make_unique<cv::Mat>(info.height,info.width,CV_8UC4,pixels);
    } catch (...) {
        AndroidBitmap_unlockPixels(env, bitmap);

        LOGE("jni_bitmap", "loadMatFromRGBA8888Bitmap caught an exception");
        jclass je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, "Android Bitmap exception in JNI code {loadMatFromRGBA8888Bitmap}");

        return nullptr;
    }
}

void releaseBitmapLock(JNIEnv *env, jobject bitmap) {
    AndroidBitmap_unlockPixels(env, bitmap);
}