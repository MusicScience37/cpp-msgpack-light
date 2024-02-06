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
 * \brief Definition of to_big_endian function.
 */
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace msgpack_light::details {

/*!
 * \brief Convert to big endian.
 *
 * \tparam UIntType Type of unsigned integer in N bytes.
 * \tparam FastUIntType Type of unsigned integer for calculation.
 * \tparam N Number of bytes.
 * \param[in] from Input.
 * \param[out] to Output.
 */
template <typename UIntType, typename FastUIntType, std::size_t N>
inline void to_big_endian_impl(
    const void* from, std::array<unsigned char, N>* to) noexcept {
    UIntType value_buffer;
    std::memcpy(&value_buffer, from, sizeof(value_buffer));
    auto value = static_cast<FastUIntType>(value_buffer);
    for (std::size_t i = 0U; i < N; ++i) {
        (*to)[N - 1U - i] =
            static_cast<unsigned char>(value & 0xFFU);  // NOLINT
        value >>= 8U;                                   // NOLINT
    }
}

/*!
 * \brief Convert to big endian.
 *
 * \tparam N Number of bytes.
 * \param[in] from Input.
 * \param[out] to Output.
 */
template <std::size_t N>
inline void to_big_endian(
    const void* from, std::array<unsigned char, N>* to) noexcept;

/*!
 * \brief Convert to big endian.
 *
 * \param[in] from Input.
 * \param[out] to Output.
 */
template <>
inline void to_big_endian<2U>(
    const void* from, std::array<unsigned char, 2U>* to) noexcept {
    to_big_endian_impl<std::uint16_t, std::uint_fast16_t, 2U>(from, to);
}

}  // namespace msgpack_light::details
