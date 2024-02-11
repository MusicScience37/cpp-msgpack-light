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
    std::uint16_t value{};
    std::memcpy(&value, from, sizeof(value));
    (*to)[0U] = static_cast<unsigned char>(value >> 8U);  // NOLINT
    (*to)[1U] = static_cast<unsigned char>(value);        // NOLINT
}

/*!
 * \brief Convert to big endian.
 *
 * \param[in] from Input.
 * \param[out] to Output.
 */
template <>
inline void to_big_endian<4U>(
    const void* from, std::array<unsigned char, 4U>* to) noexcept {
    std::uint32_t value{};
    std::memcpy(&value, from, sizeof(value));
    (*to)[0U] = static_cast<unsigned char>(value >> 24U);  // NOLINT
    (*to)[1U] = static_cast<unsigned char>(value >> 16U);  // NOLINT
    (*to)[2U] = static_cast<unsigned char>(value >> 8U);   // NOLINT
    (*to)[3U] = static_cast<unsigned char>(value);         // NOLINT
}

/*!
 * \brief Convert to big endian.
 *
 * \param[in] from Input.
 * \param[out] to Output.
 */
template <>
// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
inline void to_big_endian<8U>(
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    const void* from, std::array<unsigned char, 8U>* to) noexcept {
    std::uint64_t value{};
    std::memcpy(&value, from, sizeof(value));
    (*to)[0U] = static_cast<unsigned char>(value >> 56U);  // NOLINT
    (*to)[1U] = static_cast<unsigned char>(value >> 48U);  // NOLINT
    (*to)[2U] = static_cast<unsigned char>(value >> 40U);  // NOLINT
    (*to)[3U] = static_cast<unsigned char>(value >> 32U);  // NOLINT
    (*to)[4U] = static_cast<unsigned char>(value >> 24U);  // NOLINT
    (*to)[5U] = static_cast<unsigned char>(value >> 16U);  // NOLINT
    (*to)[6U] = static_cast<unsigned char>(value >> 8U);   // NOLINT
    (*to)[7U] = static_cast<unsigned char>(value);         // NOLINT
}

}  // namespace msgpack_light::details
