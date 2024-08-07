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
 * \brief Definition of non_buffered_serialization_buffer_impl class.
 */
#pragma once

#include <array>
#include <cstddef>  // IWYU pragma: keep

#include "msgpack_light/details/mutable_static_binary_view.h"
#include "msgpack_light/details/pack_in_big_endian.h"
#include "msgpack_light/details/total_size_of.h"  // IWYU pragma: keep
#include "msgpack_light/output_stream.h"

namespace msgpack_light::details {

/*!
 * \brief Class to implement internal implementation of serialization_buffer
 * class without buffers.
 */
class non_buffered_serialization_buffer_impl {
public:
    /*!
     * \brief Constructor.
     *
     * \param[out] stream Stream to write output to.
     *
     * \warning This class hold the reference of the given stream.
     */
    explicit non_buffered_serialization_buffer_impl(output_stream& stream)
        : stream_(stream) {}

    non_buffered_serialization_buffer_impl(
        const non_buffered_serialization_buffer_impl&) = delete;
    non_buffered_serialization_buffer_impl(
        non_buffered_serialization_buffer_impl&&) = delete;
    non_buffered_serialization_buffer_impl& operator=(
        const non_buffered_serialization_buffer_impl&) = delete;
    non_buffered_serialization_buffer_impl& operator=(
        non_buffered_serialization_buffer_impl&&) = delete;

    /*!
     * \brief Destructor.
     */
    ~non_buffered_serialization_buffer_impl() noexcept { flush(); }

    /*!
     * \brief Flush the internal buffer in this instance.
     */
    void flush() {
        // No operation.
    }

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

    /*!
     * \brief Write values in big endian.
     *
     * \tparam T Types of the values.
     * \param[in] values Values.
     */
    template <typename... T>
    void write_in_big_endian(T... values) {
        std::array<unsigned char, total_size_of<T...>> buffer{};
        pack_in_big_endian(
            mutable_static_binary_view<total_size_of<T...>>(buffer.data()),
            values...);
        write(buffer.data(), buffer.size());
    }

private:
    //! Stream to write output to.
    output_stream& stream_;
};

}  // namespace msgpack_light::details
