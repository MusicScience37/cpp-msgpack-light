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
 * \brief Definition of buffered_serialization_buffer_impl class.
 */
#pragma once

#include <array>
#include <cstddef>  // IWYU pragma: keep
#include <cstdint>
#include <cstring>
#include <limits>
#include <stdexcept>

#include "msgpack_light/details/mutable_static_binary_view.h"
#include "msgpack_light/details/pack_in_big_endian.h"
#include "msgpack_light/details/static_memory_buffer_size.h"
#include "msgpack_light/details/to_big_endian.h"
#include "msgpack_light/details/total_size_of.h"
#include "msgpack_light/output_stream.h"
#include "msgpack_light/serialization_buffer_fwd.h"  // IWYU pragma: keep
#include "msgpack_light/type_support/fwd.h"

namespace msgpack_light::details {

/*!
 * \brief Class to implement internal implementation of serialization_buffer
 * class using buffers.
 */
class buffered_serialization_buffer_impl {
public:
    /*!
     * \brief Constructor.
     *
     * \param[out] stream Stream to write output to.
     *
     * \warning This class hold the reference of the given stream.
     */
    explicit buffered_serialization_buffer_impl(output_stream& stream)
        : stream_(stream) {}

    buffered_serialization_buffer_impl(
        const buffered_serialization_buffer_impl&) = delete;
    buffered_serialization_buffer_impl(
        buffered_serialization_buffer_impl&&) = delete;
    buffered_serialization_buffer_impl& operator=(
        const buffered_serialization_buffer_impl&) = delete;
    buffered_serialization_buffer_impl& operator=(
        buffered_serialization_buffer_impl&&) = delete;

    /*!
     * \brief Destructor.
     */
    ~buffered_serialization_buffer_impl() noexcept { flush(); }

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

    /*!
     * \brief Write values in big endian.
     *
     * \tparam T Types of the values.
     * \param[in] values Values.
     */
    template <typename... T>
    inline void write_in_big_endian(T... values) {
        details::pack_in_big_endian(
            prepare_buffer<total_size_of<T...>>(), values...);
        set_buffer_written<total_size_of<T...>>();
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

}  // namespace msgpack_light::details
