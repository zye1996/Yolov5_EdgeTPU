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

#ifndef DARWINN_DRIVER_INTERRUPT_GROUPED_INTERRUPT_CONTROLLER_H_
#define DARWINN_DRIVER_INTERRUPT_GROUPED_INTERRUPT_CONTROLLER_H_

#include <memory>
#include <vector>

#include "driver/interrupt/interrupt_controller.h"
#include "driver/interrupt/interrupt_controller_interface.h"
#include "port/status.h"

namespace platforms {
namespace darwinn {
namespace driver {

// Helper class for enabling/disabling interrupts, and clearing interrupt
// status.
class GroupedInterruptController : public InterruptControllerInterface {
 public:
  // "interrupt_controllers" will be empty after construction.
  explicit GroupedInterruptController(
      std::vector<std::unique_ptr<InterruptControllerInterface>>*
          interrupt_controllers);

  // This class is neither copyable nor movable.
  GroupedInterruptController(const GroupedInterruptController&) = delete;
  GroupedInterruptController& operator=(const GroupedInterruptController&) =
      delete;

  ~GroupedInterruptController() = default;

  // Enable/disables interrupts.
  util::Status EnableInterrupts() override;
  util::Status DisableInterrupts() override;

  // Clears interrupt status register to notify that host has received the
  // interrupt.
  util::Status ClearInterruptStatus(int id) override;

 private:
  // CSR offsets.
  const std::vector<std::unique_ptr<InterruptControllerInterface>>
      interrupt_controllers_;
};

}  // namespace driver
}  // namespace darwinn
}  // namespace platforms

#endif  // DARWINN_DRIVER_INTERRUPT_GROUPED_INTERRUPT_CONTROLLER_H_
