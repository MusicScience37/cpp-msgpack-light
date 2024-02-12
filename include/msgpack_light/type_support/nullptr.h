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
 * \brief Definition of classes to support serialization of nullptr.
 */
#pragma once

#include <cstddef>

#include "msgpack_light/serialization_buffer.h"
#include "msgpack_light/type_support/fwd.h"

namespace msgpack_light::type_support {

/*!
 * \brief Class to serialize nullptr.
 */
template <>
struct serialization_traits<std::nullptr_t> {
public:
    /*!
     * \brief Serialize a value.
     *
     * \param[out] buffer Buffer.
     */
    static void serialize(
        serialization_buffer& buffer, std::nullptr_t /*value*/) {
        buffer.serialize_nil();
    }
};

}  // namespace msgpack_light::type_support
