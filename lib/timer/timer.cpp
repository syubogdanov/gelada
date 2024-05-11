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

#include "lib/timer/timer.hpp"

#include <cassert>
#include <cstddef>
#include <stdexcept>

namespace timer {

void Timer::finish(void) {
    std::lock_guard<std::recursive_mutex> lock(this->rmutex_);

    if (!this->is_started()) {
        constexpr auto detail = "The timer has not been started yet";
        throw std::runtime_error(detail);
    }

    if (this->is_finished()) {
        constexpr auto detail = "The timer has already been finished";
        throw std::runtime_error(detail);
    }

    this->is_finished_ = true;
    this->finished_at_ = std::chrono::high_resolution_clock::now();
}

bool Timer::is_finished(void) const {
    std::lock_guard<std::recursive_mutex> lock(this->rmutex_);
    return this->is_finished_;
}

bool Timer::is_started(void) const {
    std::lock_guard<std::recursive_mutex> lock(this->rmutex_);
    return this->is_started_;
}

void Timer::reset(void) {
    std::lock_guard<std::recursive_mutex> lock(this->rmutex_);

    this->is_started_ = false;
    this->is_finished_ = false;
}

void Timer::start(void) {
    std::lock_guard<std::recursive_mutex> lock(this->rmutex_);

    if (this->is_started()) {
        constexpr auto detail = "The timer has already been started";
        throw std::runtime_error(detail);
    }

    this->is_started_ = true;
    this->started_at_ = std::chrono::high_resolution_clock::now();
}

std::ostream& operator<<(std::ostream& stream, const Timer& timer) {
    timer.rmutex_.lock();

    if (!timer.is_finished()) {
        constexpr auto detail = "The timer has not been finished yet";
        throw std::runtime_error(detail);
    }

    auto difference = timer.finished_at_ - timer.started_at_;

    timer.rmutex_.unlock();

    auto duration = std::chrono::duration_cast<std::chrono::seconds>(difference).count();
    assert(duration > 0);

    auto text = std::format(
        "{} hours {} minutes {} seconds",
        duration / 60 / 60,
        duration / 60 % 60,
        duration % 60);

    return stream << text;
}

}  // namespace timer
