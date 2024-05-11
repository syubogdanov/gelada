//  ██████╗ ███████╗██╗      █████╗ ██████╗  █████╗
// ██╔════╝ ██╔════╝██║     ██╔══██╗██╔══██╗██╔══██╗
// ██║  ███╗█████╗  ██║     ███████║██║  ██║███████║
// ██║   ██║██╔══╝  ██║     ██╔══██║██║  ██║██╔══██║
// ╚██████╔╝███████╗███████╗██║  ██║██████╔╝██║  ██║
//  ╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝╚═════╝ ╚═╝  ╚═╝
//
// Copyright 2024 Sergei Bogdanov <syubogdanov@outlook.com>
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

#ifndef LIB_TIMER_TIMER_HPP_
#define LIB_TIMER_TIMER_HPP_

#include <chrono>
#include <mutex>
#include <ostream>

namespace timer {

class Timer {
 public:
    void finish(void);

    bool is_finished(void) const;

    bool is_started(void) const;

    void reset(void);

    void start(void);

    friend std::ostream& operator<<(std::ostream& stream, const timer::Timer& timer);

 private:
    mutable std::recursive_mutex rmutex_{};

    bool is_started_ = false;
    bool is_finished_ = false;

    std::chrono::high_resolution_clock::time_point started_at_;
    std::chrono::high_resolution_clock::time_point finished_at_;
};

}  // namespace timer

#endif  // LIB_TIMER_TIMER_HPP_
