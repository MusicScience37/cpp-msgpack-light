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
 * \brief Definition of classes to support serialization of binaries in STL
 * containers.
 */
#pragma once

#include "msgpack_light/serialization_buffer.h"

namespace msgpack_light::type_support::details {

/*!
 * \brief Class to serialize binaries in STL containers.
 *
 * \tparam T Type of the container.
 */
template <typename T>
struct general_binary_container_traits {
public:
    static_assert(std::is_same_v<typename T::value_type, unsigned char>);

    /*!
     * \brief Serialize a value.
     *
     * \param[out] buffer Buffer.
     * \param[in] value Value.
     */
    static void serialize(serialization_buffer& buffer, const T& value) {
        buffer.serialize_bin_size(value.size());
        buffer.write(value.data(), value.size());
    }
};

}  // namespace msgpack_light::type_support::details
