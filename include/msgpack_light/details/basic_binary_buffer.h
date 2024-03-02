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
 * \brief Definition of basic_binary_buffer class.
 */
#pragma once

#include <cstddef>  // IWYU pragma: keep
#include <cstdlib>
#include <cstring>
#include <new>
#include <utility>

namespace msgpack_light::details {

/*!
 * \brief Class of basic buffers for binary data.
 *
 * \warning This class is for internal implementations in this library,
 * so omits some checks to avoid undefined behaviors or performance degradation
 * intentionally with an assumption that the user of this class will avoid
 * such conditions.
 */
class basic_binary_buffer {
public:
    /*!
     * \brief Constructor.
     *
     * \param[in] size Size of the buffer.
     */
    explicit basic_binary_buffer(std::size_t size)
        : buffer_(static_cast<unsigned char*>(
              std::malloc(prevent_unsafe_size(size)))),
          size_(size) {
        if (buffer_ == nullptr) {
            throw std::bad_alloc();
        }
    }

    /*!
     * \brief Copy constructor.
     *
     * \param[in] other Instance to copy from.
     */
    basic_binary_buffer(const basic_binary_buffer& other)
        : basic_binary_buffer(other.size()) {
        std::memcpy(buffer_, other.buffer_, other.size_);
    }

    /*!
     * \brief Move constructor.
     *
     * \param[in,out] other Instance to move from.
     */
    basic_binary_buffer(basic_binary_buffer&& other) noexcept
        : buffer_(std::exchange(other.buffer_, nullptr)), size_(other.size_) {}

    /*!
     * \brief Copy assignment operator.
     *
     * \param[in] other Instance to copy from.
     * \return This instance after copy.
     */
    basic_binary_buffer& operator=(const basic_binary_buffer& other) {
        if (this == &other) {
            return *this;
        }
        if (size_ != other.size_) {
            resize(other.size());
        }
        std::memcpy(buffer_, other.buffer_, other.size_);
        return *this;
    }

    /*!
     * \brief Move assignment operator.
     *
     * \param[in,out] other Instance to move from.
     * \return This instance after move.
     */
    basic_binary_buffer& operator=(basic_binary_buffer&& other) noexcept {
        swap(other);
        return *this;
    }

    /*!
     * \brief Destructor.
     */
    ~basic_binary_buffer() { std::free(buffer_); }

    /*!
     * \brief Change the size of the buffer.
     *
     * \param[in] new_size New size of the buffer.
     *
     * \warning This function always call std::realloc even when the argument is
     * equal to the current size.
     */
    void resize(std::size_t new_size) {
        auto* new_buffer = static_cast<unsigned char*>(
            std::realloc(buffer_, prevent_unsafe_size(new_size)));
        if (new_buffer == nullptr) {
            throw std::bad_alloc();
        }
        buffer_ = new_buffer;
        size_ = new_size;
    }

    /*!
     * \brief Swap with another instance.
     *
     * \param[in,out] other Instance to swap with.
     */
    void swap(basic_binary_buffer& other) noexcept {
        std::swap(buffer_, other.buffer_);
        std::swap(size_, other.size_);
    }

    /*!
     * \brief Get the pointer to the buffer.
     *
     * \return Pointer to the buffer.
     */
    [[nodiscard]] unsigned char* data() noexcept { return buffer_; }

    /*!
     * \brief Get the pointer to the buffer.
     *
     * \return Pointer to the buffer.
     */
    [[nodiscard]] const unsigned char* data() const noexcept { return buffer_; }

    /*!
     * \brief Get the size of the buffer.
     *
     * \return Size of the buffer.
     */
    [[nodiscard]] std::size_t size() const noexcept { return size_; }

private:
    /*!
     * \brief Change the input to a size which won't cause
     * implementation-defined behaviors of std::malloc and std::realloc
     * functions.
     *
     * \param[in] size Size.
     * \return Size to use in std::malloc and std::realloc functions.
     */
    [[nodiscard]] static std::size_t prevent_unsafe_size(std::size_t size) {
        if (size == 0U) {
            return 1U;
        }
        return size;
    }

    //! Buffer.
    unsigned char* buffer_;

    //! Size of the buffer.
    std::size_t size_;
};

}  // namespace msgpack_light::details

namespace std {

/*!
 * \brief Implementation of std::swap for
 * msgpack_light::details::basic_binary_buffer.
 *
 * \param[in,out] instance1 An instance.
 * \param[in,out] instance2 Another instance.
 */
template <>
inline void
swap(  // NOLINT(readability-inconsistent-declaration-parameter-name)
    // It's impossible to fix this because different implementation of STL can
    // have different argument names.
    msgpack_light::details::basic_binary_buffer& instance1,
    msgpack_light::details::basic_binary_buffer& instance2) noexcept {
    instance1.swap(instance2);
}

}  // namespace std
