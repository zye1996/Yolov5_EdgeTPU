// Copyright 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef DARWINN_PORT_DEFS_H_
#define DARWINN_PORT_DEFS_H_

// Preprocessor definitions to enable/disable individual feature
// based on the selected platform:
// - DARWINN_PORT_GOOGLE3: google3
// - DARWINN_PORT_DEFAULT: no google3 deps but third_party
// - DARWINN_PORT_ANDROID_SYSTEM: no google3 deps, use external libraries

#if DARWINN_PORT_DEFAULT
#define DARWINN_PORT_USE_GOOGLE3 0
#define DARWINN_PORT_USE_EXTERNAL 0
#endif  // DARWINN_PORT_DEFAULT

#if DARWINN_PORT_FIRMWARE
#define DARWINN_PORT_USE_GOOGLE3 0
#define DARWINN_PORT_USE_EXTERNAL 0
#endif  // DARWINN_PORT_FIRMWARE

#if DARWINN_PORT_ANDROID_SYSTEM || DARWINN_PORT_ANDROID_EMULATOR
#define DARWINN_PORT_USE_GOOGLE3 0
#define DARWINN_PORT_USE_EXTERNAL 1
#endif  // DARWINN_PORT_ANDROID_SYSTEM || DARWINN_PORT_ANDROID_EMULATOR

#if DARWINN_PORT_GOOGLE3
#define DARWINN_PORT_USE_GOOGLE3 1
#define DARWINN_PORT_USE_EXTERNAL 0
#endif  // DARWINN_PORT_GOOGLE3

#endif  // DARWINN_PORT_DEFS_H_
