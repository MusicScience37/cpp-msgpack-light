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
 * \brief Definition of classes to support serialization of std::pair objects.
 */
#pragma once

#include <utility>

#include "msgpack_light/serialization_buffer.h"
#include "msgpack_light/type_support/fwd.h"

namespace msgpack_light::type_support {

/*!
 * \brief Class to serialize std::pair objects.
 *
 * \tparam T1 First type.
 * \tparam T2 Second type.
 */
template <typename T1, typename T2>
struct serialization_traits<std::pair<T1, T2>> {
public:
    /*!
     * \brief Serialize a value.
     *
     * \param[out] buffer Buffer.
     * \param[in] value Value.
     */
    static void serialize(
        serialization_buffer& buffer, const std::pair<T1, T2>& value) {
        constexpr auto size = static_cast<std::uint8_t>(2);
        buffer.serialize_fixarray_size(size);
        buffer.serialize(value.first);
        buffer.serialize(value.second);
    }
};

}  // namespace msgpack_light::type_support
