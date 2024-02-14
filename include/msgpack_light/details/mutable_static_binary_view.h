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
 * \brief Definition of mutable_static_binary_view class.
 */
#pragma once

#include <array>
#include <cstddef>

namespace msgpack_light::details {

/*!
 * \brief Class of views of mutable buffers with static sizes.
 *
 * \tparam N Number of elements.
 */
template <std::size_t N>
class mutable_static_binary_view {
public:
    /*!
     * \brief Constructor.
     *
     * \param[in] buffer Buffer.
     */
    explicit mutable_static_binary_view(unsigned char* buffer) noexcept
        : buffer_(buffer) {}

    /*!
     * \brief Access to a byte.
     *
     * \param[in] index Index of the byte.
     * \return Reference to the byte.
     */
    unsigned char& operator[](std::size_t index) noexcept {
        return buffer_[index];
    }

    /*!
     * \brief Get a byte.
     *
     * \param[in] index Index of the byte.
     * \return Value of the byte.
     */
    unsigned char operator[](std::size_t index) const noexcept {
        return buffer_[index];
    }

    /*!
     * \brief Access the buffer.
     *
     * \return Buffer.
     */
    unsigned char* data() noexcept { return buffer_; }

    /*!
     * \brief Get the size of the buffer.
     *
     * \return Size.
     */
    [[nodiscard]] std::size_t size() const noexcept { return N; }

private:
    //! Buffer.
    unsigned char* buffer_;
};

}  // namespace msgpack_light::details
