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

extern "C" const char kDarwinnRuntimeBuildEmbedLabel[];
const char kDarwinnRuntimeBuildEmbedLabel[] = BUILD_EMBED_LABEL;

extern "C" const char kDarwinnRuntimeBaseChangeList[];
const char kDarwinnRuntimeBaseChangeList[] = "CL_NUMBER=339933739";

namespace {
// Build a type whose constructor will contain references to all the build data
// variables, preventing them from being GC'ed by the linker.
struct KeepBuildDataVariables {
  KeepBuildDataVariables() {
    volatile int opaque_flag = 0;
    if (!opaque_flag) return;

    const void* volatile capture;
    capture = &kDarwinnRuntimeBuildEmbedLabel;
    capture = &kDarwinnRuntimeBaseChangeList;
    static_cast<void>(capture);
  }
} dummy;
}  // namespace
