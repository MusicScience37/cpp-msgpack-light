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
 * \brief Definition of pack_in_big_endian function.
 */
#pragma once

#include "msgpack_light/details/mutable_static_binary_view.h"
#include "msgpack_light/details/to_big_endian.h"
#include "msgpack_light/details/total_size_of.h"  // IWYU pragma: keep

namespace msgpack_light::details {

/*!
 * \brief Pack some values in big endian.
 *
 * \tparam T First type.
 * \tparam Remaining Remaining types.
 * \param[out] buffer Buffer.
 * \param[in] value First value.
 * \param[in] remaining_values Remaining values.
 */
template <typename T, typename... Remaining>
void pack_in_big_endian(
    mutable_static_binary_view<total_size_of<T, Remaining...>> buffer, T value,
    Remaining... remaining_values) noexcept {
    if constexpr (sizeof(T) == 1U) {
        buffer[0] = static_cast<unsigned char>(value);
    } else {
        if constexpr (sizeof...(Remaining) == 0U) {
            to_big_endian(&value, buffer);
        } else {
            to_big_endian(&value, buffer.template sub_buffer<0U, sizeof(T)>());
        }
    }
    if constexpr (sizeof...(Remaining) > 0U) {
        pack_in_big_endian(
            buffer.template sub_buffer<sizeof(T)>(), remaining_values...);
    }
}

}  // namespace msgpack_light::details
