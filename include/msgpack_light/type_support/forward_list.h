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
 * \brief Definition of classes to support serialization of std::forward_list
 * instances.
 */
#pragma once

#include <cstddef>
#include <forward_list>
#include <iterator>

#include "msgpack_light/serialization_buffer.h"
#include "msgpack_light/type_support/fwd.h"

namespace msgpack_light::type_support {

/*!
 * \brief Class to serialize std::forward_list instances.
 *
 * \tparam T Type of elements.
 * \tparam Allocator Type of allocators.
 */
template <typename T, typename Allocator>
struct serialization_traits<std::forward_list<T, Allocator>> {
public:
    /*!
     * \brief Serialize a value.
     *
     * \param[out] buffer Buffer.
     * \param[in] value Value.
     */
    static void serialize(serialization_buffer& buffer,
        const std::forward_list<T, Allocator>& value) {
        buffer.serialize_array_size(static_cast<std::size_t>(
            std::distance(value.begin(), value.end())));
        for (const auto& elem : value) {
            buffer.serialize(elem);
        }
    }
};

}  // namespace msgpack_light::type_support
