/* Copyright 2015 The CodingMaster Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef CODINGMASTER_PLATFORM_MACROS_H_
#define CODINGMASTER_PLATFORM_MACROS_H_

#if defined(__clang__) && (!defined(SWIG))
#define THREAD_ANNOTATION_ATTRIBUTE__(x)   __attribute__((x))
#else
#define THREAD_ANNOTATION_ATTRIBUTE__(x)   // no-op
#endif

// Document if a shared variable/field needs to be protected by a mutex.
// GUARDED_BY allows the user to specify a particular mutex that should be
// held when accessing the annotated variable.  GUARDED_VAR indicates that
// a shared variable is guarded by some unspecified mutex, for use in rare
// cases where a valid mutex expression cannot be specified.
#define GUARDED_BY(x) THREAD_ANNOTATION_ATTRIBUTE__(guarded_by(x))
 
// CHECK dies with a fatal error if condition is not true.  It is *not*
// controlled by NDEBUG, so the check will be executed regardless of
// compilation mode.  Therefore, it is safe to do things like:
//    CHECK(fp->Write(x) == 4)
#define CHECK(condition)              \
  if (TF_PREDICT_FALSE(!(condition))) \
  LOG(FATAL) << "Check failed: " #condition " "

// Compilers can be told that a certain branch is not likely to be taken
// (for instance, a CHECK failure), and use that information in static
// analysis. Giving it this information can help it optimize for the
// common case in the absence of better information (ie.
// -fprofile-arcs).
//
// We need to disable this for GPU builds, though, since nvcc8 and older
// don't recognize `__builtin_expect` as a builtin, and fail compilation.
#if (!defined(__NVCC__)) && \
    (TF_HAS_BUILTIN(__builtin_expect) || (defined(__GNUC__) && __GNUC__ >= 3))
#define TF_PREDICT_FALSE(x) (__builtin_expect(x, 0)) // vc: expect to be false to hint compiler
#define TF_PREDICT_TRUE(x) (__builtin_expect(!!(x), 1))
#else
#define TF_PREDICT_FALSE(x) (x)
#define TF_PREDICT_TRUE(x) (x)
#endif

//
// The expression TF_ARRAYSIZE(a) is a compile-time constant of type
// size_t.
// vc: 27 byte array
// 27 / 6 = 4 => first line
// / 
// static_cast<size_t>(!(27 % 6)) = > 3 is retured and negate 0xFF FF FF FC 
// A 10, B 11, C 12, D 13, E 14, F 15
// when it can have extra? if aligned array
// 
#define TF_ARRAYSIZE(a)         \
  ((sizeof(a) / sizeof(*(a))) / \
   static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

// A macro to disallow the copy constructor and operator= functions
// This is usually placed in the private: declarations for a class.
#define CM_DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;         \
  void operator=(const TypeName&) = delete

#define N_ALIGNED_MALLOC(typeName,size,n) \
  	void* tempAddress = malloc((size) + ((n)-1)); \
  	static_case<typeName>(tempAddress & (~((n)-1)));

#define EXPORT_API __attribute__(visibility("default")) // vc: -fvisibility=hidden

#endif  // CODINGMASTER_PLATFORM_MACROS_H_
