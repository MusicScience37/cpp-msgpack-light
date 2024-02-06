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
 * \brief Definition of serialization_buffer class.
 */
#pragma once

#include <array>
#include <cstdint>

#include "msgpack_light/details/to_big_endian.h"
#include "msgpack_light/output_stream.h"
#include "msgpack_light/serialization_buffer_fwd.h"
#include "msgpack_light/type_support/fwd.h"

namespace msgpack_light {

/*!
 * \brief Class of buffers to serialize data.
 */
class serialization_buffer {
public:
    /*!
     * \brief Constructor.
     *
     * \param[out] stream Stream to write output to.
     *
     * \warning This class hold the reference of the given stream.
     */
    explicit serialization_buffer(output_stream& stream) : stream_(stream) {}

    /*!
     * \brief Serialize a boolean value.
     *
     * \param[in] value Value.
     */
    void serialize_bool(bool value) {
        constexpr auto false_byte = static_cast<unsigned char>(0xC2);
        constexpr auto true_byte = static_cast<unsigned char>(0xC3);
        if (value) {
            put(true_byte);
        } else {
            put(false_byte);
        }
    }

    /*!
     * \brief Serialize a value in positive fixint format.
     *
     * \warning This function assumes that the value is in the range of 0 to
     * `0x7F`.
     *
     * \param[in] value Value.
     */
    void serialize_positive_fixint(std::uint8_t value) {
        put(static_cast<unsigned char>(value));
    }

    /*!
     * \brief Serialize a value in negative fixint format.
     *
     * \warning This function assumes that the value is in the range of `-1` to
     * `0xE0`.
     *
     * \param[in] value Value.
     */
    void serialize_negative_fixint(std::uint8_t value) {
        put(static_cast<unsigned char>(value));
    }

    /*!
     * \brief Serialize a value in uint 8 format.
     *
     * \param[in] value Value.
     */
    void serialize_uint8(std::uint8_t value) {
        constexpr auto prefix = static_cast<unsigned char>(0xCC);
        put(prefix);
        put(static_cast<unsigned char>(value));
    }

    /*!
     * \brief Serialize a value in uint 16 format.
     *
     * \param[in] value Value.
     */
    void serialize_uint16(std::uint16_t value) {
        constexpr auto prefix = static_cast<unsigned char>(0xCD);
        put(prefix);
        std::array<unsigned char, 2U> buffer{};
        details::to_big_endian(&value, &buffer);
        write(buffer.data(), buffer.size());
    }

    /*!
     * \brief Serialize a value in uint 32 format.
     *
     * \param[in] value Value.
     */
    void serialize_uint32(std::uint32_t value) {
        constexpr auto prefix = static_cast<unsigned char>(0xCE);
        put(prefix);
        std::array<unsigned char, 4U> buffer{};
        details::to_big_endian(&value, &buffer);
        write(buffer.data(), buffer.size());
    }

    /*!
     * \brief Serialize a value in uint 64 format.
     *
     * \param[in] value Value.
     */
    void serialize_uint64(std::uint64_t value) {
        constexpr auto prefix = static_cast<unsigned char>(0xCF);
        put(prefix);
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        std::array<unsigned char, 8U> buffer{};
        details::to_big_endian(&value, &buffer);
        write(buffer.data(), buffer.size());
    }

    /*!
     * \brief Serialize a value in int 8 format.
     *
     * \param[in] value Value.
     */
    void serialize_int8(std::int8_t value) {
        constexpr auto prefix = static_cast<unsigned char>(0xD0);
        put(prefix);
        put(static_cast<unsigned char>(value));
    }

    /*!
     * \brief Serialize a value in int 16 format.
     *
     * \param[in] value Value.
     */
    void serialize_int16(std::int16_t value) {
        constexpr auto prefix = static_cast<unsigned char>(0xD1);
        put(prefix);
        std::array<unsigned char, 2U> buffer{};
        details::to_big_endian(&value, &buffer);
        write(buffer.data(), buffer.size());
    }

    /*!
     * \brief Serialize data.
     *
     * \tparam T Type of data.
     * \param[in] data Data.
     */
    template <typename T>
    void serialize(const T& data) {
        type_support::serialization_traits<T>::serialize(*this, data);
    }

private:
    /*!
     * \brief Write data.
     *
     * \param[in] data Pointer to the data.
     * \param[in] size Size of the data.
     */
    void write(const unsigned char* data, std::size_t size) {
        stream_.write(data, size);
    }

    /*!
     * \brief Write a byte of data.
     *
     * \param[in] data Data.
     */
    void put(unsigned char data) { write(&data, 1U); }

    //! Stream to write output to.
    output_stream& stream_;
};

}  // namespace msgpack_light
