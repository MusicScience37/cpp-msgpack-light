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
#include <cstddef>  // IWYU pragma: keep
#include <cstdint>
#include <cstring>
#include <limits>
#include <stdexcept>

#include "msgpack_light/details/mutable_static_binary_view.h"
#include "msgpack_light/details/static_memory_buffer_size.h"
#include "msgpack_light/details/to_big_endian.h"
#include "msgpack_light/output_stream.h"
#include "msgpack_light/serialization_buffer_fwd.h"  // IWYU pragma: keep
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

    serialization_buffer(const serialization_buffer&) = delete;
    serialization_buffer(serialization_buffer&&) = delete;
    serialization_buffer& operator=(const serialization_buffer&) = delete;
    serialization_buffer& operator=(serialization_buffer&&) = delete;

    /*!
     * \brief Destructor.
     */
    ~serialization_buffer() noexcept { flush(); }

    /*!
     * \brief Serialize a nli value.
     */
    void serialize_nil() {
        constexpr auto nil_byte = static_cast<unsigned char>(0xC0);
        put(nil_byte);
    }

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
        write_in_big_endian(value);
    }

    /*!
     * \brief Serialize a value in uint 32 format.
     *
     * \param[in] value Value.
     */
    void serialize_uint32(std::uint32_t value) {
        constexpr auto prefix = static_cast<unsigned char>(0xCE);
        put(prefix);
        write_in_big_endian(value);
    }

    /*!
     * \brief Serialize a value in uint 64 format.
     *
     * \param[in] value Value.
     */
    void serialize_uint64(std::uint64_t value) {
        constexpr auto prefix = static_cast<unsigned char>(0xCF);
        put(prefix);
        write_in_big_endian(value);
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
        write_in_big_endian(value);
    }

    /*!
     * \brief Serialize a value in int 32 format.
     *
     * \param[in] value Value.
     */
    void serialize_int32(std::int32_t value) {
        constexpr auto prefix = static_cast<unsigned char>(0xD2);
        put(prefix);
        write_in_big_endian(value);
    }

    /*!
     * \brief Serialize a value in int 64 format.
     *
     * \param[in] value Value.
     */
    void serialize_int64(std::int64_t value) {
        constexpr auto prefix = static_cast<unsigned char>(0xD3);
        put(prefix);
        write_in_big_endian(value);
    }

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
        put(prefix);
        write_in_big_endian(value);
    }

    /*!
     * \brief Serialize a value in float 64 format.
     *
     * \param[in] value Value.
     */
    void serialize_float64(double value) {
        static_assert(std::numeric_limits<double>::is_iec559,
            "IEEE 754 double precision floating point number is required.");
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        static_assert(sizeof(double) == 8U,
            "IEEE 754 double precision floating point number is required.");

        constexpr auto prefix = static_cast<unsigned char>(0xCB);
        put(prefix);
        write_in_big_endian(value);
    }

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
        put(prefix);
        put(static_cast<unsigned char>(size));
    }

    /*!
     * \brief Serialize a size of str 16 format.
     *
     * \param[in] size Size.
     */
    void serialize_str16_size(std::uint16_t size) {
        constexpr auto prefix = static_cast<unsigned char>(0xDA);
        put(prefix);
        write_in_big_endian(size);
    }

    /*!
     * \brief Serialize a size of str 32 format.
     *
     * \param[in] size Size.
     */
    void serialize_str32_size(std::uint32_t size) {
        constexpr auto prefix = static_cast<unsigned char>(0xDB);
        put(prefix);
        write_in_big_endian(size);
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

    /*!
     * \brief Serialize a size of bin 8 format.
     *
     * \param[in] size Size.
     */
    void serialize_bin8_size(std::uint8_t size) {
        constexpr auto prefix = static_cast<unsigned char>(0xC4);
        put(prefix);
        put(static_cast<unsigned char>(size));
    }

    /*!
     * \brief Serialize a size of bin 16 format.
     *
     * \param[in] size Size.
     */
    void serialize_bin16_size(std::uint16_t size) {
        constexpr auto prefix = static_cast<unsigned char>(0xC5);
        put(prefix);
        write_in_big_endian(size);
    }

    /*!
     * \brief Serialize a size of bin 32 format.
     *
     * \param[in] size Size.
     */
    void serialize_bin32_size(std::uint32_t size) {
        constexpr auto prefix = static_cast<unsigned char>(0xC6);
        put(prefix);
        write_in_big_endian(size);
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
        put(prefix);
        write_in_big_endian(size);
    }

    /*!
     * \brief Serialize a size of array 32 format.
     *
     * \param[in] size Size.
     */
    void serialize_array32_size(std::uint32_t size) {
        constexpr auto prefix = static_cast<unsigned char>(0xDD);
        put(prefix);
        write_in_big_endian(size);
    }

    /*!
     * \brief Serialize a size of an array.
     *
     * \param[in] size Size.
     */
    void serialize_array_size(std::size_t size) {
        constexpr auto max_fixarray_size = static_cast<std::size_t>(15);
        constexpr auto max_array16_size = static_cast<std::size_t>(0xFFFF);

        if (size <= max_fixarray_size) {
            serialize_fixarray_size(static_cast<std::uint8_t>(size));
            return;
        }

        if (size <= max_array16_size) {
            serialize_array16_size(static_cast<std::uint16_t>(size));
            return;
        }

        if constexpr (sizeof(std::size_t) > 4U) {
            constexpr auto max_array32_size =
                static_cast<std::size_t>(0xFFFFFFFF);
            if (size > max_array32_size) {
                throw std::runtime_error("Size is too large.");
            }
        }

        serialize_array32_size(static_cast<std::uint32_t>(size));
    }

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
        put(prefix);
        write_in_big_endian(size);
    }

    /*!
     * \brief Serialize a size of map 32 format.
     *
     * \param[in] size Size.
     */
    void serialize_map32_size(std::uint32_t size) {
        constexpr auto prefix = static_cast<unsigned char>(0xDF);
        put(prefix);
        write_in_big_endian(size);
    }

    /*!
     * \brief Serialize a size of a map.
     *
     * \param[in] size Size.
     */
    void serialize_map_size(std::size_t size) {
        constexpr auto max_fixmap_size = static_cast<std::size_t>(15);
        constexpr auto max_map16_size = static_cast<std::size_t>(0xFFFF);

        if (size <= max_fixmap_size) {
            serialize_fixmap_size(static_cast<std::uint8_t>(size));
            return;
        }

        if (size <= max_map16_size) {
            serialize_map16_size(static_cast<std::uint16_t>(size));
            return;
        }

        if constexpr (sizeof(std::size_t) > 4U) {
            constexpr auto max_map32_size =
                static_cast<std::size_t>(0xFFFFFFFF);
            if (size > max_map32_size) {
                throw std::runtime_error("Size is too large.");
            }
        }

        serialize_map32_size(static_cast<std::uint32_t>(size));
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

    /*!
     * \brief Flush the internal buffer in this object.
     */
    void flush() {
        stream_.write(buffer_.data(), current_position_in_buffer_);
        current_position_in_buffer_ = 0U;
    }

    /*!
     * \brief Write data.
     *
     * \param[in] data Pointer to the data.
     * \param[in] size Size of the data.
     */
    void write(const unsigned char* data, std::size_t size) {
        if (buffer_size - current_position_in_buffer_ < size) {
            flush();
            if (buffer_size < size) {
                stream_.write(data, size);
                return;
            }
        }

        std::memcpy(buffer_.data() + current_position_in_buffer_, data, size);
        current_position_in_buffer_ += size;
    }

    /*!
     * \brief Write a byte of data.
     *
     * \param[in] data Data.
     */
    void put(unsigned char data) {
        if (buffer_size == current_position_in_buffer_) {
            flush();
        }
        *(buffer_.data() + current_position_in_buffer_) = data;
        ++current_position_in_buffer_;
    }

private:
    /*!
     * \brief Prepare a buffer.
     *
     * \tparam N Number of bytes.
     * \return Buffer.
     */
    template <std::size_t N>
    [[nodiscard]] inline details::mutable_static_binary_view<N>
    prepare_buffer() {
        static_assert(N <= buffer_size);

        if (buffer_size - current_position_in_buffer_ < N) {
            flush();
        }

        return details::mutable_static_binary_view<N>(
            buffer_.data() + current_position_in_buffer_);
    }

    /*!
     * \brief Set the buffer returned by prepare_buffer function to be written.
     *
     * \tparam N Number of bytes.
     */
    template <std::size_t N>
    inline void set_buffer_written() noexcept {
        current_position_in_buffer_ += N;
    }

    /*!
     * \brief Write a value in big endian.
     *
     * \tparam T Type of the value.
     * \param[in] value Value.
     */
    template <typename T>
    inline void write_in_big_endian(T value) {
        details::to_big_endian(&value, prepare_buffer<sizeof(T)>());
        set_buffer_written<sizeof(T)>();
    }

    //! Stream to write output to.
    output_stream& stream_;

    //! Size of the internal buffer.
    static constexpr std::size_t buffer_size =
        details::static_memory_buffer_size;

    //! Internal buffer.
    std::array<unsigned char, buffer_size> buffer_{};

    //! Current position in the internal buffer.
    std::size_t current_position_in_buffer_{0U};
};

}  // namespace msgpack_light
