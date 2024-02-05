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
#include <cstdlib>
#include <cstring>
#include <new>

namespace msgpack_light {

/*!
 * \brief Class of streams to write data to memory.
 */
class memory_output_stream {
public:
    /*!
     * \brief Constructor.
     */
    memory_output_stream()
        : buffer_(static_cast<unsigned char*>(
              // NOLINTNEXTLINE(hicpp-no-malloc, cppcoreguidelines-no-malloc): This class is a container.
              std::malloc(initial_buffer_size))),
          capacity_(initial_buffer_size) {
        if (buffer_ == nullptr) {
            throw std::bad_alloc();
        }
    }

    memory_output_stream(const memory_output_stream&) = delete;
    memory_output_stream(memory_output_stream&&) = delete;
    memory_output_stream& operator=(const memory_output_stream&) = delete;
    memory_output_stream& operator=(memory_output_stream&&) = delete;

    /*!
     * \brief Destructor.
     */
    ~memory_output_stream() {
        // NOLINTNEXTLINE(hicpp-no-malloc, cppcoreguidelines-no-malloc): This class is a container.
        std::free(buffer_);
    }

    /*!
     * \brief Write data.
     *
     * \param[in] data Pointer to the data.
     * \param[in] size Size of the data.
     */
    void write(const unsigned char* data, std::size_t size) {
        const std::size_t new_written = written_ + size;
        if (new_written > capacity_) {
            const std::size_t new_capacity = new_written * 2U;
            auto* new_buffer = static_cast<unsigned char*>(
                // NOLINTNEXTLINE(hicpp-no-malloc, cppcoreguidelines-no-malloc): This class is a container.
                std::realloc(buffer_, new_capacity));
            if (new_buffer == nullptr) {
                throw std::bad_alloc();
            }
            buffer_ = new_buffer;
            capacity_ = new_capacity;
        }

        std::memcpy(buffer_ + written_, data, size);
        written_ = new_written;
    }

    /*!
     * \brief Get the pointer to the written data.
     *
     * \return Pointer to the written data.
     */
    [[nodiscard]] const unsigned char* data() const noexcept { return buffer_; }

    /*!
     * \brief Get the size of the written data.
     *
     * \return Size of the written data.
     */
    [[nodiscard]] std::size_t size() const noexcept { return written_; }

private:
    //! Size of the initial buffer.
    static constexpr std::size_t initial_buffer_size = 512;

    //! Pointer to the current buffer.
    unsigned char* buffer_;

    //! Size of the allocated buffer.
    std::size_t capacity_;

    //! Size of the written data.
    std::size_t written_{0U};
};

}  // namespace msgpack_light
