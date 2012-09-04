/*
 * Copyright (C) 2009 The Android Open Source Project
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

// OpenGL ES 2.0 code

#include <jni.h>
#include <android/log.h>
#include "../../../Engine/Application.h"
#include "../../../Engine/Input.h"

extern "C" {
    JNIEXPORT void JNICALL Java_com_drybones_evate_GL2JNILib_init(JNIEnv * env, jobject obj, jstring sdpath);
    JNIEXPORT void JNICALL Java_com_drybones_evate_GL2JNILib_step(JNIEnv * env, jobject obj, jfloat deltaTime);
	JNIEXPORT void JNICALL Java_com_drybones_evate_GL2JNILib_pointer(JNIEnv * env, jobject obj, jint type, jint x, jint y, jint x2, jint y2);
};

JNIEXPORT void JNICALL Java_com_drybones_evate_GL2JNILib_init(JNIEnv * env, jobject obj, jstring sdpath)
{
	LOGI("Start JNI Load");
	const char *path = env->GetStringUTFChars(sdpath, 0);
    Application::GetInstance()->Init( "/mnt/sdcard/KO2D/Resource" );
	env->ReleaseStringUTFChars(sdpath, path);
	LOGI("Finish JNI Load");
}

JNIEXPORT void JNICALL Java_com_drybones_evate_GL2JNILib_step(JNIEnv * env, jobject obj, jfloat deltaTime)
{
	LOGI("JNI deltaTime: %f", deltaTime);
    Application::GetInstance()->Render();
	Application::GetInstance()->Update(deltaTime);
}

JNIEXPORT void JNICALL Java_com_drybones_evate_GL2JNILib_pointer(JNIEnv * env, jobject obj, jint type, jint x, jint y, jint x2, jint y2)
{
	if (type == 1)
		Input::GetInstance()->OnPointer(POINTER_DOWN, x, y, x2, y2);
	if (type == 2)
		Input::GetInstance()->OnPointer(POINTER_UP, x, y, x2, y2);
}
