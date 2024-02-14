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
 * \brief Definition of classes to support serialization of std::tuple objects.
 */
#pragma once

#include <cstddef>
#include <tuple>

#include "msgpack_light/serialization_buffer.h"
#include "msgpack_light/type_support/fwd.h"

namespace msgpack_light::type_support {

/*!
 * \brief Class to serialize std::tuple objects.
 *
 * \tparam T Types in the tuple.
 */
template <typename... T>
struct serialization_traits<std::tuple<T...>> {
public:
    /*!
     * \brief Serialize a value.
     *
     * \param[out] buffer Buffer.
     * \param[in] value Value.
     */
    static void serialize(
        serialization_buffer& buffer, const std::tuple<T...>& value) {
        buffer.serialize_array_size(sizeof...(T));
        serialize_element<0>(buffer, value);
    }

private:
    /*!
     * \brief Serialize an element.
     *
     * \tparam I Index of the element.
     * \param[out] buffer Buffer.
     * \param[in] value Value.
     */
    template <std::size_t I>
    static void serialize_element(
        serialization_buffer& buffer, const std::tuple<T...>& value) {
        if constexpr (I < sizeof...(T)) {
            buffer.serialize(std::get<I>(value));
            serialize_element<I + 1>(buffer, value);
        }
    }
};

}  // namespace msgpack_light::type_support
