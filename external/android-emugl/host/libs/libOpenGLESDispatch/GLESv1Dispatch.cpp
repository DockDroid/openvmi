/*
* Copyright (C) 2011 The Android Open Source Project
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include "OpenGLESDispatch/GLESv1Dispatch.h"
#include "OpenGLESDispatch/GLESv2Dispatch.h"

//#include "android/utils/debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "emugl/common/shared_library.h"


static emugl::SharedLibrary *s_gles1_lib = NULL;

static void gles1_unimplemented() {
    fprintf(stderr, "Called unimplemented GLESv1 API\n");
}

// This section of code (also in GLDispatch.cpp)
// initializes all GLESv1 functions to dummy ones;
// that is, in case we are using a library that doesn't
// have GLESv1, we will still have stubs available to
// signal that they are unsupported on the host.

#define RETURN_void return
#define RETURN_GLboolean return GL_FALSE
#define RETURN_GLint return 0
#define RETURN_GLuint return 0U
#define RETURN_GLenum return 0
#define RETURN_int return 0
#define RETURN_GLconstubyteptr return NULL
#define RETURN_voidptr return NULL

#define RETURN_(x)  RETURN_ ## x

#define DUMMY_MSG "Call to %s: host OpenGL driver does not support OpenGL ES v1. Skipping."

#define DEFINE_DUMMY_FUNCTION(return_type, func_name, signature, args) \
static return_type gles1_dummy_##func_name signature { \
    fprintf(stderr, DUMMY_MSG, #func_name); \
    RETURN_(return_type); \
}

#define DEFINE_DUMMY_EXTENSION_FUNCTION(return_type, func_name, signature, args) \
static return_type gles1_dummy_##func_name signature { \
    fprintf(stderr, DUMMY_MSG, #func_name); \
    RETURN_(return_type); \
}



LIST_GLES1_FUNCTIONS(DEFINE_DUMMY_FUNCTION, DEFINE_DUMMY_EXTENSION_FUNCTION);


//
// This function is called only once during initialiation before
// any thread has been created - hence it should NOT be thread safe.
//

//
// init dummy GLESv1 dispatch table
//
#define ASSIGN_DUMMY(return_type,function_name,signature,callargs) do { \
        dispatch_table-> function_name = gles1_dummy_##function_name; \
        } while(0);

bool gles1_dispatch_init(const char *path, GLESv1Dispatch* dispatch_table) {
//    dispatch_table->underlying_gles2_api = NULL;
    // If no path is given we assign dummy functions to all GL calls
    // we would have loaded from a real implementation.
//add by zyp 2020.5.15
    if (!dispatch_table)
        return false;

    if (!path) {
        LIST_GLES1_FUNCTIONS(ASSIGN_DUMMY, ASSIGN_DUMMY);
        return true;
    }

    const char* libName = getenv("OPENVMI_GLESv1_LIB");
    if (!libName)
      libName = path;
    if (!libName)
        return false;

    char error[256];
    s_gles1_lib = emugl::SharedLibrary::open(libName, error, sizeof(error));
    if (!s_gles1_lib) {
        fprintf(stderr, "%s: Could not load %s [%s]\n", __FUNCTION__,
                libName, error);
        return false;
    }

    //
    // init the GLES dispatch table
    //
#define LOOKUP_SYMBOL(return_type,function_name,signature,callargs) do { \
        dispatch_table-> function_name = reinterpret_cast< function_name ## _t >( \
                s_gles1_lib->findSymbol(#function_name)); \
        } while(0);

    LIST_GLES1_FUNCTIONS(LOOKUP_SYMBOL,LOOKUP_SYMBOL)


   dispatch_table->initialized = true;
   return true;

}

//
// This function is called only during initialization of the decoder before
// any thread has been created - hence it should NOT be thread safe.
//
void *gles1_dispatch_get_proc_func(const char *name, void *userData)
{
    void* func = NULL;
    if (s_gles1_lib) {
        func = (void *)s_gles1_lib->findSymbol(name);
    }
    // To make it consistent with the guest, redirect any unsupported functions
    // to gles1_unimplemented.
    if (!func) {
        func = (void *)gles1_unimplemented;
    }
    return func;
}
