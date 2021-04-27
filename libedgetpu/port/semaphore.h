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

#ifndef DARWINN_PORT_SEMAPHORE_H_
#define DARWINN_PORT_SEMAPHORE_H_

#include "port/defs.h"

#if DARWINN_PORT_FIRMWARE
#include "third_party/safertos_addons/semaphore.h"

namespace platforms {
namespace darwinn {

using BinarySemaphore = safertos_addons::BinarySemaphore;
using CountingSemaphore = safertos_addons::CountingSemaphore;

}  // namespace darwinn
}  // namespace platforms

#else
#include "port/default/semaphore.h"

#endif  // DARWINN_PORT_DEFAULT
#endif  // DARWINN_PORT_SEMAPHORE_H_
