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
 * \brief Definition of classes to support serialization of strings.
 */
#pragma once

#include <string>
#include <string_view>

#include "msgpack_light/serialization_buffer.h"
#include "msgpack_light/type_support/fwd.h"

namespace msgpack_light::type_support {

namespace details {

/*!
 * \brief Class to serialize strings.
 *
 * \tparam T Type of strings.
 */
template <typename T>
struct general_string_serialization_traits {
public:
    /*!
     * \brief Serialize a value.
     *
     * \param[out] buffer Buffer.
     * \param[in] value Value.
     */
    static void serialize(serialization_buffer& buffer, const T& value) {
        buffer.serialize_str_size(value.size());
        buffer.write(static_cast<const unsigned char*>(
                         static_cast<const void*>(value.data())),
            value.size());
    }
};

}  // namespace details

/*!
 * \brief Class to serialize strings.
 */
template <typename Allocator>
struct serialization_traits<
    std::basic_string<char, std::char_traits<char>, Allocator>>
    : public details::general_string_serialization_traits<
          std::basic_string<char, std::char_traits<char>, Allocator>> {};

/*!
 * \brief Class to serialize strings.
 */
template <>
struct serialization_traits<std::string_view>
    : public details::general_string_serialization_traits<std::string_view> {};

}  // namespace msgpack_light::type_support
