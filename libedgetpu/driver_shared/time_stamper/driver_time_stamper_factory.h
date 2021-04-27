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

#ifndef DARWINN_DRIVER_SHARED_TIME_STAMPER_DRIVER_TIME_STAMPER_FACTORY_H_
#define DARWINN_DRIVER_SHARED_TIME_STAMPER_DRIVER_TIME_STAMPER_FACTORY_H_

#include "driver_shared/time_stamper/driver_time_stamper.h"
#include "driver_shared/time_stamper/time_stamper.h"
#include "driver_shared/time_stamper/time_stamper_factory.h"
#include "port/ptr_util.h"

namespace platforms {
namespace darwinn {

namespace driver_shared {

// Factory class for creating DriverTimeStamper objects.
class DriverTimeStamperFactory : public TimeStamperFactory {
 public:
  DriverTimeStamperFactory() = default;
  virtual ~DriverTimeStamperFactory() = default;

  std::unique_ptr<TimeStamper> CreateTimeStamper() override {
    return gtl::MakeUnique<DriverTimeStamper>();
  }
};

}  // namespace driver_shared
}  // namespace darwinn
}  // namespace platforms

#endif  // DARWINN_DRIVER_SHARED_TIME_STAMPER_DRIVER_TIME_STAMPER_FACTORY_H_
