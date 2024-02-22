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
 * \brief Definition of memory_output_stream class.
 */
#pragma once

#include <cstddef>

#include "msgpack_light/binary.h"
#include "msgpack_light/details/static_memory_buffer_size.h"
#include "msgpack_light/output_stream.h"

namespace msgpack_light {

/*!
 * \brief Class of streams to write data to memory.
 */
class memory_output_stream final : public output_stream {
public:
    /*!
     * \brief Constructor.
     */
    memory_output_stream() { buffer_.reserve(initial_buffer_size); }

    /*!
     * \brief Write data.
     *
     * \param[in] data Pointer to the data.
     * \param[in] size Size of the data.
     */
    void write(const unsigned char* data, std::size_t size) override {
        buffer_.append(data, size);
    }

    /*!
     * \brief Clear data.
     */
    void clear() { buffer_.resize(0U); }

    /*!
     * \brief Get the pointer to the written data.
     *
     * \return Pointer to the written data.
     */
    [[nodiscard]] const unsigned char* data() const noexcept {
        return buffer_.data();
    }

    /*!
     * \brief Get the size of the written data.
     *
     * \return Size of the written data.
     */
    [[nodiscard]] std::size_t size() const noexcept { return buffer_.size(); }

    /*!
     * \brief Get the data as msgpack_light::binary object.
     *
     * \return binary object.
     */
    [[nodiscard]] const binary& as_binary() const { return buffer_; }

private:
    //! Size of the initial buffer.
    static constexpr std::size_t initial_buffer_size = 4096U;

    static_assert(initial_buffer_size > details::static_memory_buffer_size);

    //! Buffer.
    binary buffer_{};
};

}  // namespace msgpack_light
