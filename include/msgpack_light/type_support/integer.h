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
 * \brief Definition of classes to support serialization of integer values.
 */
#pragma once

#include <cstdint>
#include <type_traits>

#include "msgpack_light/serialization_buffer.h"
#include "msgpack_light/type_support/fwd.h"

namespace msgpack_light::type_support {

namespace details {

/*!
 * \brief Check whether a type is an integer.
 *
 * \tparam T Type to check.
 */
template <typename T>
constexpr bool is_integer_v = std::is_integral_v<T> && !std::is_same_v<T, bool>;

/*!
 * \brief Check whether a type is a signed integer.
 *
 * \tparam T Type to check.
 */
template <typename T>
constexpr bool is_signed_integer_v = is_integer_v<T> && std::is_signed_v<T>;

/*!
 * \brief Check whether a type is an unsigned integer.
 *
 * \tparam T Type to check.
 */
template <typename T>
constexpr bool is_unsigned_integer_v = is_integer_v<T> && std::is_unsigned_v<T>;

/*!
 * \brief Maximum value in positive fixint format.
 *
 * \tparam T Type of the integer.
 */
template <typename T>
constexpr auto max_positive_fixint = static_cast<T>(0x7F);

/*!
 * \brief Maximum value in uint 8 format.
 *
 * \tparam T Type of the integer.
 */
template <typename T>
constexpr auto max_uint8 = static_cast<T>(0xFF);

/*!
 * \brief Maximum value in uint 16 format.
 *
 * \tparam T Type of the integer.
 */
template <typename T>
constexpr auto max_uint16 = static_cast<T>(0xFFFF);

/*!
 * \brief Maximum value in uint 32 format.
 *
 * \tparam T Type of the integer.
 */
template <typename T>
constexpr auto max_uint32 = static_cast<T>(0xFFFFFFFF);

/*!
 * \brief Minimum value in negative fixint format.
 *
 * \tparam T Type of the integer.
 */
template <typename T>
constexpr auto min_negative_fixint =
    // NOLINTNEXTLINE: false positive
    static_cast<T>(static_cast<std::int8_t>(0b11100000));

/*!
 * \brief Minimum value in int 8 format.
 *
 * \tparam T Type of the integer.
 */
template <typename T>
constexpr auto min_int8 =
    // NOLINTNEXTLINE: false positive
    static_cast<T>(static_cast<std::int8_t>(0X80));

/*!
 * \brief Minimum value in int 16 format.
 *
 * \tparam T Type of the integer.
 */
template <typename T>
constexpr auto min_int16 = static_cast<T>(static_cast<std::int16_t>(0X8000));

/*!
 * \brief Minimum value in int 32 format.
 *
 * \tparam T Type of the integer.
 */
template <typename T>
constexpr auto min_int32 =
    static_cast<T>(static_cast<std::int32_t>(0X80000000));

}  // namespace details

/*!
 * \brief Class to serialize unsigned integers.
 *
 * \tparam T Type of the unsigned integer.
 */
template <typename T>
struct serialization_traits<T,
    std::enable_if_t<details::is_unsigned_integer_v<T> &&
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        sizeof(T) <= 8U>> {
    /*!
     * \brief Serialize a value.
     *
     * \param[out] buffer Buffer.
     * \param[in] value Value.
     */
    static void serialize(serialization_buffer& buffer, T value) {
        constexpr T positive_fixint_mask = ~details::max_positive_fixint<T>;
        if ((value & positive_fixint_mask) == static_cast<T>(0)) {
            buffer.serialize_positive_fixint(static_cast<std::uint8_t>(value));
            return;
        }

        if constexpr (sizeof(T) == 1U) {
            buffer.serialize_uint8(static_cast<std::uint8_t>(value));
            return;
        } else {
            constexpr T uint8_mask = ~details::max_uint8<T>;
            if ((value & uint8_mask) == static_cast<T>(0)) {
                buffer.serialize_uint8(static_cast<std::uint8_t>(value));
                return;
            }
        }

        if constexpr (sizeof(T) == 2U) {
            buffer.serialize_uint16(static_cast<std::uint16_t>(value));
            return;
        } else if constexpr (sizeof(T) > 2U) {
            constexpr T uint16_mask = ~details::max_uint16<T>;
            if ((value & uint16_mask) == static_cast<T>(0)) {
                buffer.serialize_uint16(static_cast<std::uint16_t>(value));
                return;
            }
        }

        if constexpr (sizeof(T) == 4U) {
            buffer.serialize_uint32(static_cast<std::uint32_t>(value));
            return;
        } else if constexpr (sizeof(T) > 4U) {
            constexpr T uint32_mask = ~details::max_uint32<T>;
            if ((value & uint32_mask) == static_cast<T>(0)) {
                buffer.serialize_uint32(static_cast<std::uint32_t>(value));
                return;
            }
            buffer.serialize_uint64(static_cast<std::uint64_t>(value));
        }
    }
};

/*!
 * \brief Class to serialize signed integers.
 *
 * \tparam T Type of the signed integer.
 */
template <typename T>
struct serialization_traits<T,
    std::enable_if_t<details::is_signed_integer_v<T> &&
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        sizeof(T) <= 8U>> {
public:
    /*!
     * \brief Serialize a value.
     *
     * \param[out] buffer Buffer.
     * \param[in] value Value.
     */
    static void serialize(serialization_buffer& buffer, T value) {
        if (value >= static_cast<T>(0)) {
            serialize_positive_integer(buffer, value);
        } else {
            serialize_negative_integer(buffer, value);
        }
    }

private:
    /*!
     * \brief Serialize a positive integers.
     *
     * \param[out] buffer Buffer.
     * \param[in] value Value.
     */
    static void serialize_positive_integer(
        serialization_buffer& buffer, T value) {
        if constexpr (sizeof(T) == 1U) {
            buffer.serialize_positive_fixint(static_cast<std::uint8_t>(value));
            return;
        } else {
            constexpr T positive_fixint_mask = ~details::max_positive_fixint<T>;
            if ((value & positive_fixint_mask) == static_cast<T>(0)) {
                buffer.serialize_positive_fixint(
                    static_cast<std::uint8_t>(value));
                return;
            }
            constexpr T uint8_mask = ~details::max_uint8<T>;
            if ((value & uint8_mask) == static_cast<T>(0)) {
                buffer.serialize_uint8(static_cast<std::uint8_t>(value));
                return;
            }
        }

        if constexpr (sizeof(T) == 2U) {
            buffer.serialize_uint16(static_cast<std::uint16_t>(value));
            return;
        } else if constexpr (sizeof(T) > 2U) {
            constexpr T uint16_mask = ~details::max_uint16<T>;
            if ((value & uint16_mask) == static_cast<T>(0)) {
                buffer.serialize_uint16(static_cast<std::uint16_t>(value));
                return;
            }
        }

        if constexpr (sizeof(T) == 4U) {
            buffer.serialize_uint32(static_cast<std::uint32_t>(value));
            return;
        } else if constexpr (sizeof(T) > 4U) {
            constexpr T uint32_mask = ~details::max_uint32<T>;
            if ((value & uint32_mask) == static_cast<T>(0)) {
                buffer.serialize_uint32(static_cast<std::uint32_t>(value));
                return;
            }
            buffer.serialize_uint64(static_cast<std::uint64_t>(value));
        }
    }

    /*!
     * \brief Serialize a negative integers.
     *
     * \param[out] buffer Buffer.
     * \param[in] value Value.
     */
    static void serialize_negative_integer(
        serialization_buffer& buffer, T value) {
        constexpr T negative_fixint_mask = details::min_negative_fixint<T>;
        if ((value & negative_fixint_mask) == negative_fixint_mask) {
            buffer.serialize_negative_fixint(static_cast<std::int8_t>(value));
            return;
        }

        if constexpr (sizeof(T) == 1U) {
            buffer.serialize_int8(static_cast<std::int8_t>(value));
            return;
        } else {
            constexpr T int8_mask = details::min_int8<T>;
            if ((value & int8_mask) == int8_mask) {
                buffer.serialize_int8(static_cast<std::int8_t>(value));
                return;
            }
        }

        if constexpr (sizeof(T) == 2U) {
            buffer.serialize_int16(static_cast<std::int16_t>(value));
            return;
        } else if constexpr (sizeof(T) > 2U) {
            constexpr T int16_mask = details::min_int16<T>;
            if ((value & int16_mask) == int16_mask) {
                buffer.serialize_int16(static_cast<std::int16_t>(value));
                return;
            }
        }

        if constexpr (sizeof(T) == 4U) {
            buffer.serialize_int32(static_cast<std::int32_t>(value));
            return;
        } else if constexpr (sizeof(T) > 4U) {
            constexpr T int32_mask = details::min_int32<T>;
            if ((value & int32_mask) == int32_mask) {
                buffer.serialize_int32(static_cast<std::int32_t>(value));
                return;
            }

            buffer.serialize_int64(static_cast<std::int64_t>(value));
        }
    }
};

}  // namespace msgpack_light::type_support
