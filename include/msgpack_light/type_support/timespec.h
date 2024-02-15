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
 * \brief Definition of classes to support serialization of std::timespec
 * objects.
 */
#pragma once

#include <cstdint>
#include <ctime>

#include "msgpack_light/serialization_buffer.h"
#include "msgpack_light/type_support/fwd.h"

namespace msgpack_light::type_support {

/*!
 * \brief Class to serialize std::timespec objects
 */
template <>
struct serialization_traits<std::timespec> {
public:
    /*!
     * \brief Serialize a value.
     *
     * \warning This function assumes that `value.tv_nsec` is in the range from
     * 0 to 999999999.
     *
     * \param[out] buffer Buffer.
     * \param[in] value Value.
     */
    static void serialize(
        serialization_buffer& buffer, const std::timespec& value) {
        constexpr auto ext_type = static_cast<std::int8_t>(-1);

        constexpr auto max_timestamp32_seconds =
            static_cast<std::time_t>(0xFFFFFFFF);
        if (value.tv_nsec == 0 &&
            static_cast<std::time_t>(0U) <= value.tv_sec &&
            value.tv_sec <= max_timestamp32_seconds) {
            // timestamp 32 format
            buffer.serialize_fixext4_header(ext_type);
            buffer.write_in_big_endian(
                static_cast<std::uint32_t>(value.tv_sec));
            return;
        }

        constexpr auto max_timestamp64_seconds =
            static_cast<std::time_t>(0x3FFFFFFFF);
        if (static_cast<std::time_t>(0U) <= value.tv_sec &&
            value.tv_sec <= max_timestamp64_seconds) {
            // timestamp 64 format
            auto data = static_cast<std::uint64_t>(value.tv_nsec);
            constexpr unsigned int nsec_offset = 34;
            data <<= nsec_offset;
            data |= static_cast<std::uint64_t>(value.tv_sec);

            buffer.serialize_fixext8_header(ext_type);
            buffer.write_in_big_endian(data);
            return;
        }

        // timestamp 96 format
        constexpr std::size_t timestamp96_data_size = 12;
        buffer.serialize_ext_header(ext_type, timestamp96_data_size);
        buffer.write_in_big_endian(static_cast<std::uint32_t>(value.tv_nsec));
        buffer.write_in_big_endian(static_cast<std::int64_t>(value.tv_sec));
    }
};

}  // namespace msgpack_light::type_support
