/*
 * Copyright 2024 MusicScience37 (Kenta Kabashima)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*!
 * \file
 * \brief Definition of classes to support serialization of time.
 */
#pragma once

#include <chrono>
#include <ctime>

#include "msgpack_light/serialization_buffer.h"
#include "msgpack_light/type_support/fwd.h"
#include "msgpack_light/type_support/timespec.h"

namespace msgpack_light::type_support {

/*!
 * \brief Class to serialize std::time_point instances.
 *
 * \tparam Duration Type of the duration in std::time_point class.
 */
template <typename Duration>
struct serialization_traits<
    std::chrono::time_point<std::chrono::system_clock, Duration>> {
public:
    /*!
     * \brief Serialize a value.
     *
     * \param[out] buffer Buffer.
     * \param[in] value Value.
     */
    static void serialize(serialization_buffer& buffer,
        std::chrono::time_point<std::chrono::system_clock, Duration> value) {
        const auto seconds_time_point = std::chrono::time_point_cast<
            std::chrono::duration<typename Duration::rep>>(value);
        const auto nanoseconds =
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                value - seconds_time_point)
                .count();

        std::timespec value_timespec{};
        value_timespec.tv_sec =
            std::chrono::system_clock::to_time_t(seconds_time_point);
        // NOLINTNEXTLINE(google-runtime-int): This type is determined by C++ standard.
        value_timespec.tv_nsec = static_cast<long>(nanoseconds);

        serialization_traits<std::timespec>::serialize(buffer, value_timespec);
    }
};

}  // namespace msgpack_light::type_support
