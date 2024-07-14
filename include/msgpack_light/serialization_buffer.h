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

#include <cstddef>  // IWYU pragma: keep
#include <cstdint>
#include <limits>
#include <stdexcept>

#include "msgpack_light/details/serialization_buffer_impl.h"
#include "msgpack_light/output_stream.h"
#include "msgpack_light/serialization_buffer_fwd.h"  // IWYU pragma: keep
#include "msgpack_light/type_support/fwd.h"

namespace msgpack_light {

/*!
 * \brief Class of buffers to serialize data.
 *
 * \note For serialization of types already supported by
 * msgpack_light::type_support::serialization_traits template, use serialize()
 * function.
 */
class serialization_buffer {
public:
    /*!
     * \name Initialization and finalization.
     */
    //!\{

    /*!
     * \brief Constructor.
     *
     * \param[out] stream Stream to write output to.
     *
     * \warning This class hold the reference of the given stream.
     */
    explicit serialization_buffer(output_stream& stream) : buffer_(stream) {}

    serialization_buffer(const serialization_buffer&) = delete;
    serialization_buffer(serialization_buffer&&) = delete;
    serialization_buffer& operator=(const serialization_buffer&) = delete;
    serialization_buffer& operator=(serialization_buffer&&) = delete;

    /*!
     * \brief Destructor.
     *
     * \note This will call flush() function.
     */
    ~serialization_buffer() noexcept { flush(); }

    /*!
     * \brief Flush the internal buffer in this instance.
     *
     * \warning Data may not be written to streams without call of this function
     * or destructor.
     */
    void flush() { buffer_.flush(); }

    //!\}

    /*!
     * \name Serialization of Nil in MessagePack.
     */
    //!\{

    /*!
     * \brief Serialize a nli value.
     */
    void serialize_nil() {
        constexpr auto nil_byte = static_cast<unsigned char>(0xC0);
        put(nil_byte);
    }

    //!\}

    /*!
     * \name Serialization of Boolean in MessagePack.
     */
    //!\{

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

    //!\}

    /*!
     * \name Serialization of Integer in MessagePack.
     *
     * These functions implements serialization of integers in formats in
     * MessagePack specification.
     * To serialize integers with automatic selection of formats, use
     * serialize() function.
     */
    //!\{

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
    void serialize_negative_fixint(std::int8_t value) {
        put(static_cast<unsigned char>(value));
    }

    /*!
     * \brief Serialize a value in uint 8 format.
     *
     * \param[in] value Value.
     */
    void serialize_uint8(std::uint8_t value) {
        constexpr auto prefix = static_cast<unsigned char>(0xCC);
        write_in_big_endian(prefix, value);
    }

    /*!
     * \brief Serialize a value in uint 16 format.
     *
     * \param[in] value Value.
     */
    void serialize_uint16(std::uint16_t value) {
        constexpr auto prefix = static_cast<unsigned char>(0xCD);
        write_in_big_endian(prefix, value);
    }

    /*!
     * \brief Serialize a value in uint 32 format.
     *
     * \param[in] value Value.
     */
    void serialize_uint32(std::uint32_t value) {
        constexpr auto prefix = static_cast<unsigned char>(0xCE);
        write_in_big_endian(prefix, value);
    }

    /*!
     * \brief Serialize a value in uint 64 format.
     *
     * \param[in] value Value.
     */
    void serialize_uint64(std::uint64_t value) {
        constexpr auto prefix = static_cast<unsigned char>(0xCF);
        write_in_big_endian(prefix, value);
    }

    /*!
     * \brief Serialize a value in int 8 format.
     *
     * \param[in] value Value.
     */
    void serialize_int8(std::int8_t value) {
        constexpr auto prefix = static_cast<unsigned char>(0xD0);
        write_in_big_endian(prefix, value);
    }

    /*!
     * \brief Serialize a value in int 16 format.
     *
     * \param[in] value Value.
     */
    void serialize_int16(std::int16_t value) {
        constexpr auto prefix = static_cast<unsigned char>(0xD1);
        write_in_big_endian(prefix, value);
    }

    /*!
     * \brief Serialize a value in int 32 format.
     *
     * \param[in] value Value.
     */
    void serialize_int32(std::int32_t value) {
        constexpr auto prefix = static_cast<unsigned char>(0xD2);
        write_in_big_endian(prefix, value);
    }

    /*!
     * \brief Serialize a value in int 64 format.
     *
     * \param[in] value Value.
     */
    void serialize_int64(std::int64_t value) {
        constexpr auto prefix = static_cast<unsigned char>(0xD3);
        write_in_big_endian(prefix, value);
    }

    //!\}

    /*!
     * \name Serialization of Float in MessagePack.
     */
    //!\{

    /*!
     * \brief Serialize a value in float 32 format.
     *
     * \param[in] value Value.
     */
    void serialize_float32(float value) {
        static_assert(std::numeric_limits<float>::is_iec559,
            "IEEE 754 single precision floating point number is required.");
        static_assert(sizeof(float) == 4U,
            "IEEE 754 single precision floating point number is required.");

        constexpr auto prefix = static_cast<unsigned char>(0xCA);
        write_in_big_endian(prefix, value);
    }

    /*!
     * \brief Serialize a value in float 64 format.
     *
     * \param[in] value Value.
     */
    void serialize_float64(double value) {
        static_assert(std::numeric_limits<double>::is_iec559,
            "IEEE 754 double precision floating point number is required.");
        // NOLINTNEXTLINE(readability-magic-numbers)
        static_assert(sizeof(double) == 8U,
            "IEEE 754 double precision floating point number is required.");

        constexpr auto prefix = static_cast<unsigned char>(0xCB);
        write_in_big_endian(prefix, value);
    }

    //!\}

    /*!
     * \name Serialization of sizes in String in MessagePack.
     *
     * To serialize sizes with automatic selection of formats, use
     * serialize_str_size() function.
     */
    //!\{

    /*!
     * \brief Serialize a size of fixstr format.
     *
     * \warning This function assumes that the size is in the range of 0 to
     * `0x1F`.
     *
     * \param[in] size Size.
     */
    void serialize_fixstr_size(std::uint8_t size) {
        constexpr auto prefix = static_cast<unsigned char>(0xA0);
        put(prefix | size);
    }

    /*!
     * \brief Serialize a size of str 8 format.
     *
     * \param[in] size Size.
     */
    void serialize_str8_size(std::uint8_t size) {
        constexpr auto prefix = static_cast<unsigned char>(0xD9);
        write_in_big_endian(prefix, size);
    }

    /*!
     * \brief Serialize a size of str 16 format.
     *
     * \param[in] size Size.
     */
    void serialize_str16_size(std::uint16_t size) {
        constexpr auto prefix = static_cast<unsigned char>(0xDA);
        write_in_big_endian(prefix, size);
    }

    /*!
     * \brief Serialize a size of str 32 format.
     *
     * \param[in] size Size.
     */
    void serialize_str32_size(std::uint32_t size) {
        constexpr auto prefix = static_cast<unsigned char>(0xDB);
        write_in_big_endian(prefix, size);
    }

    /*!
     * \brief Serialize a size of a string.
     *
     * \param[in] size Size.
     */
    void serialize_str_size(std::size_t size) {
        constexpr auto max_fixstr_size = static_cast<std::size_t>(0b11111U);
        constexpr auto max_str8_size = static_cast<std::size_t>(0xFF);
        constexpr auto max_str16_size = static_cast<std::size_t>(0xFFFF);

        if (size <= max_str8_size) {
            if (size <= max_fixstr_size) {
                serialize_fixstr_size(static_cast<std::uint8_t>(size));
                return;
            }

            serialize_str8_size(static_cast<std::uint8_t>(size));
            return;
        }

        if (size <= max_str16_size) {
            serialize_str16_size(static_cast<std::uint16_t>(size));
            return;
        }

        if constexpr (sizeof(std::size_t) > 4U) {
            constexpr auto max_str32_size =
                static_cast<std::size_t>(0xFFFFFFFF);
            if (size > max_str32_size) {
                throw std::runtime_error("Size is too large.");
            }
        }

        serialize_str32_size(static_cast<std::uint32_t>(size));
    }

    //!\}

    /*!
     * \name Serialization of sizes in Binary in MessagePack.
     *
     * To serialize sizes with automatic selection of formats, use
     * serialize_bin_size() function.
     */
    //!\{

    /*!
     * \brief Serialize a size of bin 8 format.
     *
     * \param[in] size Size.
     */
    void serialize_bin8_size(std::uint8_t size) {
        constexpr auto prefix = static_cast<unsigned char>(0xC4);
        write_in_big_endian(prefix, size);
    }

    /*!
     * \brief Serialize a size of bin 16 format.
     *
     * \param[in] size Size.
     */
    void serialize_bin16_size(std::uint16_t size) {
        constexpr auto prefix = static_cast<unsigned char>(0xC5);
        write_in_big_endian(prefix, size);
    }

    /*!
     * \brief Serialize a size of bin 32 format.
     *
     * \param[in] size Size.
     */
    void serialize_bin32_size(std::uint32_t size) {
        constexpr auto prefix = static_cast<unsigned char>(0xC6);
        write_in_big_endian(prefix, size);
    }

    /*!
     * \brief Serialize a size of a binary.
     *
     * \param[in] size Size.
     */
    void serialize_bin_size(std::size_t size) {
        constexpr auto max_bin8_size = static_cast<std::size_t>(0xFF);
        constexpr auto max_bin16_size = static_cast<std::size_t>(0xFFFF);

        if (size <= max_bin8_size) {
            serialize_bin8_size(static_cast<std::uint8_t>(size));
            return;
        }

        if (size <= max_bin16_size) {
            serialize_bin16_size(static_cast<std::uint16_t>(size));
            return;
        }

        if constexpr (sizeof(std::size_t) > 4U) {
            constexpr auto max_bin32_size =
                static_cast<std::size_t>(0xFFFFFFFF);
            if (size > max_bin32_size) {
                throw std::runtime_error("Size is too large.");
            }
        }

        serialize_bin32_size(static_cast<std::uint32_t>(size));
    }

    //!\}

    /*!
     * \name Serialization of sizes in Array in MessagePack.
     *
     * To serialize sizes with automatic selection of formats, use
     * serialize_array_size() function.
     */
    //!\{

    /*!
     * \brief Serialize a size of fixarray format.
     *
     * \warning This function assumes that the size is in the range of 0 to 15.
     *
     * \param[in] size Size.
     */
    void serialize_fixarray_size(std::uint8_t size) {
        constexpr auto prefix = static_cast<unsigned char>(0x90);
        put(prefix | size);
    }

    /*!
     * \brief Serialize a size of array 16 format.
     *
     * \param[in] size Size.
     */
    void serialize_array16_size(std::uint16_t size) {
        constexpr auto prefix = static_cast<unsigned char>(0xDC);
        write_in_big_endian(prefix, size);
    }

    /*!
     * \brief Serialize a size of array 32 format.
     *
     * \param[in] size Size.
     */
    void serialize_array32_size(std::uint32_t size) {
        constexpr auto prefix = static_cast<unsigned char>(0xDD);
        write_in_big_endian(prefix, size);
    }

    /*!
     * \brief Serialize a size of an array.
     *
     * \param[in] size Size.
     */
    void serialize_array_size(std::size_t size) {
        constexpr auto fixarray_mask = ~static_cast<std::size_t>(15);
        if ((size & fixarray_mask) == static_cast<std::size_t>(0)) {
            serialize_fixarray_size(static_cast<std::uint8_t>(size));
            return;
        }

        constexpr auto array16_mask = ~static_cast<std::size_t>(0xFFFF);
        if ((size & array16_mask) == static_cast<std::size_t>(0)) {
            serialize_array16_size(static_cast<std::uint16_t>(size));
            return;
        }

        if constexpr (sizeof(std::size_t) > 4U) {
            constexpr auto array32_mask = ~static_cast<std::size_t>(0xFFFFFFFF);
            if ((size & array32_mask) != static_cast<std::size_t>(0)) {
                throw std::runtime_error("Size is too large.");
            }
        }

        serialize_array32_size(static_cast<std::uint32_t>(size));
    }

    //!\}

    /*!
     * \name Serialization of sizes in Map in MessagePack.
     *
     * To serialize sizes with automatic selection of formats, use
     * serialize_map_size() function.
     */
    //!\{

    /*!
     * \brief Serialize a size of fixmap format.
     *
     * \warning This function assumes that the size is in the range of 0 to 15.
     *
     * \param[in] size Size.
     */
    void serialize_fixmap_size(std::uint8_t size) {
        constexpr auto prefix = static_cast<unsigned char>(0x80);
        put(prefix | size);
    }

    /*!
     * \brief Serialize a size of map 16 format.
     *
     * \param[in] size Size.
     */
    void serialize_map16_size(std::uint16_t size) {
        constexpr auto prefix = static_cast<unsigned char>(0xDE);
        write_in_big_endian(prefix, size);
    }

    /*!
     * \brief Serialize a size of map 32 format.
     *
     * \param[in] size Size.
     */
    void serialize_map32_size(std::uint32_t size) {
        constexpr auto prefix = static_cast<unsigned char>(0xDF);
        write_in_big_endian(prefix, size);
    }

    /*!
     * \brief Serialize a size of a map.
     *
     * \param[in] size Size.
     */
    void serialize_map_size(std::size_t size) {
        constexpr auto fixmap_mask = ~static_cast<std::size_t>(15);
        if ((size & fixmap_mask) == static_cast<std::size_t>(0)) {
            serialize_fixmap_size(static_cast<std::uint8_t>(size));
            return;
        }

        constexpr auto map16_mask = ~static_cast<std::size_t>(0xFFFF);
        if ((size & map16_mask) == static_cast<std::size_t>(0)) {
            serialize_map16_size(static_cast<std::uint16_t>(size));
            return;
        }

        if constexpr (sizeof(std::size_t) > 4U) {
            constexpr auto map32_mask = ~static_cast<std::size_t>(0xFFFFFFFF);
            if ((size & map32_mask) != static_cast<std::size_t>(0)) {
                throw std::runtime_error("Size is too large.");
            }
        }

        serialize_map32_size(static_cast<std::uint32_t>(size));
    }

    //!\}

    /*!
     * \name Serialization of sizes and types in Extension in MessagePack.
     *
     * To serialize sizes with automatic selection of formats, use
     * serialize_ext_header() function.
     */
    //!\{

    /*!
     * \brief Serialize the size and type of an extension value in fixext 1
     * format.
     *
     * \note Serialize data of the value after call of this function.
     *
     * \param[in] ext_type Extension type.
     */
    void serialize_fixext1_header(std::int8_t ext_type) {
        constexpr auto prefix = static_cast<unsigned char>(0xD4);
        write_in_big_endian(prefix, ext_type);
    }

    /*!
     * \brief Serialize the size and type of an extension value in fixext 2
     * format.
     *
     * \note Serialize data of the value after call of this function.
     *
     * \param[in] ext_type Extension type.
     */
    void serialize_fixext2_header(std::int8_t ext_type) {
        constexpr auto prefix = static_cast<unsigned char>(0xD5);
        write_in_big_endian(prefix, ext_type);
    }

    /*!
     * \brief Serialize the size and type of an extension value in fixext 4
     * format.
     *
     * \note Serialize data of the value after call of this function.
     *
     * \param[in] ext_type Extension type.
     */
    void serialize_fixext4_header(std::int8_t ext_type) {
        constexpr auto prefix = static_cast<unsigned char>(0xD6);
        write_in_big_endian(prefix, ext_type);
    }

    /*!
     * \brief Serialize the size and type of an extension value in fixext 8
     * format.
     *
     * \note Serialize data of the value after call of this function.
     *
     * \param[in] ext_type Extension type.
     */
    void serialize_fixext8_header(std::int8_t ext_type) {
        constexpr auto prefix = static_cast<unsigned char>(0xD7);
        write_in_big_endian(prefix, ext_type);
    }

    /*!
     * \brief Serialize the size and type of an extension value in fixext 16
     * format.
     *
     * \note Serialize data of the value after call of this function.
     *
     * \param[in] ext_type Extension type.
     */
    void serialize_fixext16_header(std::int8_t ext_type) {
        constexpr auto prefix = static_cast<unsigned char>(0xD8);
        write_in_big_endian(prefix, ext_type);
    }

    /*!
     * \brief Serialize the size and type of an extension value in ext 8 format.
     *
     * \note Serialize data of the value after call of this function.
     *
     * \param[in] ext_type Extension type.
     * \param[in] data_size Size of the data.
     */
    void serialize_ext8_header(std::int8_t ext_type, std::uint8_t data_size) {
        constexpr auto prefix = static_cast<unsigned char>(0xC7);
        write_in_big_endian(prefix, data_size, ext_type);
    }

    /*!
     * \brief Serialize the size and type of an extension value in ext 16
     * format.
     *
     * \note Serialize data of the value after call of this function.
     *
     * \param[in] ext_type Extension type.
     * \param[in] data_size Size of the data.
     */
    void serialize_ext16_header(std::int8_t ext_type, std::uint16_t data_size) {
        constexpr auto prefix = static_cast<unsigned char>(0xC8);
        write_in_big_endian(prefix, data_size, ext_type);
    }

    /*!
     * \brief Serialize the size and type of an extension value in ext 32
     * format.
     *
     * \note Serialize data of the value after call of this function.
     *
     * \param[in] ext_type Extension type.
     * \param[in] data_size Size of the data.
     */
    void serialize_ext32_header(std::int8_t ext_type, std::uint32_t data_size) {
        constexpr auto prefix = static_cast<unsigned char>(0xC9);
        write_in_big_endian(prefix, data_size, ext_type);
    }

    /*!
     * \brief Serialize the size and type of an extension value.
     *
     * \note Serialize data of the value after call of this function.
     *
     * \param[in] ext_type Extension type.
     * \param[in] data_size Size of the data.
     */
    void serialize_ext_header(std::int8_t ext_type, std::size_t data_size) {
        constexpr auto ext8_data_size_mask = ~static_cast<std::size_t>(0xFF);
        if ((data_size & ext8_data_size_mask) == static_cast<std::size_t>(0)) {
            constexpr auto fixext1_data_size = static_cast<std::size_t>(1);
            constexpr auto fixext2_data_size = static_cast<std::size_t>(2);
            constexpr auto fixext4_data_size = static_cast<std::size_t>(4);
            constexpr auto fixext8_data_size = static_cast<std::size_t>(8);
            constexpr auto fixext16_data_size = static_cast<std::size_t>(16);
            switch (data_size) {
            case fixext1_data_size:
                serialize_fixext1_header(ext_type);
                return;
            case fixext2_data_size:
                serialize_fixext2_header(ext_type);
                return;
            case fixext4_data_size:
                serialize_fixext4_header(ext_type);
                return;
            case fixext8_data_size:
                serialize_fixext8_header(ext_type);
                return;
            case fixext16_data_size:
                serialize_fixext16_header(ext_type);
                return;
            default:
                serialize_ext8_header(
                    ext_type, static_cast<std::uint8_t>(data_size));
                return;
            }
        }

        constexpr auto ext16_data_size_mask = ~static_cast<std::size_t>(0xFFFF);
        if ((data_size & ext16_data_size_mask) == static_cast<std::size_t>(0)) {
            serialize_ext16_header(
                ext_type, static_cast<std::uint16_t>(data_size));
            return;
        }

        if constexpr (sizeof(std::size_t) > 4U) {
            constexpr auto ext32_data_size_mask =
                ~static_cast<std::size_t>(0xFFFFFFFF);
            if ((data_size & ext32_data_size_mask) !=
                static_cast<std::size_t>(0)) {
                throw std::runtime_error("Size is too large.");
            }
        }

        serialize_ext32_header(ext_type, static_cast<std::uint32_t>(data_size));
    }

    //!\}

    /*!
     * \name Serialization of general types.
     */
    //!\{

    /*!
     * \brief Serialize data.
     *
     * \note This function can be usable for types using
     * msgpack_light::type_support::serialization_traits class.
     *
     * \tparam T Type of data.
     * \param[in] data Data.
     */
    template <typename T>
    void serialize(const T& data) {
        type_support::serialization_traits<T>::serialize(*this, data);
    }

    //!\}

    /*!
     * \name Functions to write data directly.
     */
    //!\{

    /*!
     * \brief Write data.
     *
     * \note Use this function to write data of strings, binaries, arrays, maps,
     * and extension types.
     *
     * \param[in] data Pointer to the data.
     * \param[in] size Size of the data.
     */
    void write(const unsigned char* data, std::size_t size) {
        buffer_.write(data, size);
    }

    /*!
     * \brief Write a byte of data.
     *
     * \param[in] data Data.
     */
    void put(unsigned char data) { buffer_.put(data); }

    /*!
     * \brief Write values in big endian.
     *
     * \tparam T Types of the values.
     * \param[in] values Values.
     */
    template <typename... T>
    void write_in_big_endian(T... values) {
        buffer_.write_in_big_endian(values...);
    }

    //!\}

private:
    //! Instance to perform internal processing.
    details::serialization_buffer_impl buffer_;
};

}  // namespace msgpack_light
