/* Copyright 2015 The TensorFlow Authors. All Rights Reserved.

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

// Various Google-specific casting templates.
//
// This code is compiled directly on many platforms, including client
// platforms like Windows, Mac, and embedded systems.  Before making
// any changes here, make sure that you're not breaking any platforms.
//

#ifndef DARWINN_PORT_DEFAULT_PORT_FROM_TF_CASTS_H_
#define DARWINN_PORT_DEFAULT_PORT_FROM_TF_CASTS_H_

#include <string.h>  // for memcpy

namespace platforms {
namespace darwinn {

// bit_cast<Dest,Source> is a template function that implements the
// equivalent of "*reinterpret_cast<Dest*>(&source)".  We need this in
// very low-level functions like the protobuf library and fast math
// support.
//
//   float f = 3.14159265358979;
//   int i = bit_cast<int32>(f);
//   // i = 0x40490fdb
//
// The classical address-casting method is:
//
//   // WRONG
//   float f = 3.14159265358979;            // WRONG
//   int i = * reinterpret_cast<int*>(&f);  // WRONG
//
// The address-casting method actually produces undefined behavior
// according to ISO C++ specification section 3.10 -15 -.  Roughly, this
// section says: if an object in memory has one type, and a program
// accesses it with a different type, then the result is undefined
// behavior for most values of "different type".
//
// This is true for any cast syntax, either *(int*)&f or
// *reinterpret_cast<int*>(&f).  And it is particularly true for
// conversions between integral lvalues and floating-point lvalues.
//
// The purpose of 3.10 -15- is to allow optimizing compilers to assume
// that expressions with different types refer to different memory.  gcc
// 4.0.1 has an optimizer that takes advantage of this.  So a
// non-conforming program quietly produces wildly incorrect output.
//
// The problem is not the use of reinterpret_cast.  The problem is type
// punning: holding an object in memory of one type and reading its bits
// back using a different type.
//
// The C++ standard is more subtle and complex than this, but that
// is the basic idea.
//
// Anyways ...
//
// bit_cast<> calls memcpy() which is blessed by the standard,
// especially by the example in section 3.9 .  Also, of course,
// bit_cast<> wraps up the nasty logic in one place.
//
// Fortunately memcpy() is very fast.  In optimized mode, with a
// constant size, gcc 2.95.3, gcc 4.0.1, and msvc 7.1 produce inline
// code with the minimal amount of data movement.  On a 32-bit system,
// memcpy(d,s,4) compiles to one load and one store, and memcpy(d,s,8)
// compiles to two loads and two stores.
//
// I tested this code with gcc 2.95.3, gcc 4.0.1, icc 8.1, and msvc 7.1.
//
// WARNING: if Dest or Source is a non-POD type, the result of the memcpy
// is likely to surprise you.
//
// Props to Bill Gibbons for the compile time assertion technique and
// Art Komninos and Igor Tandetnik for the msvc experiments.
//
// -- mec 2005-10-17

template <class Dest, class Source>
inline Dest bit_cast(const Source& source) {
  static_assert(sizeof(Dest) == sizeof(Source), "Sizes do not match");

  Dest dest;
  memcpy(&dest, &source, sizeof(dest));
  return dest;
}

// Identity metafunction.
// DEPRECATED(b/10657301): No std equivalent, but it's trivial.
// NOTE: This will be released as part of ABCL because there are some sensible
// usage. However, this will be moved along with other template stuff from GTL
// into a separate file, which resides in a directory for MPL.
// Also, this will be moved into another namespace to avoid confusion with
// other similar things like libstdc++ __gnu_cxx::identity.
template <class T>
struct identity_ {
  typedef T type;
};

// Use implicit_cast as a safe version of static_cast or const_cast
// for implicit conversions. For example:
// - Upcasting in a type hierarchy.
// - Performing arithmetic conversions (int32 to int64, int to double, etc.).
// - Adding const or volatile qualifiers.
//
// In general, implicit_cast can be used to convert this code
//   To to = from;
//   DoSomething(to);
// to this
//   DoSomething(implicit_cast<To>(from));
//
// base::identity_ is used to make a non-deduced context, which
// forces all callers to explicitly specify the template argument.
template <typename To>
inline To implicit_cast(typename identity_<To>::type to) {
  return to;
}

// This version of implicit_cast is used when two template arguments
// are specified. It's obsolete and should not be used.
template <typename To, typename From>
inline To implicit_cast(typename identity_<From>::type const& f) {
  return f;
}

}  // namespace darwinn
}  // namespace platforms

#endif  // DARWINN_PORT_DEFAULT_PORT_FROM_TF_CASTS_H_
