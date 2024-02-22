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
 * \brief Definition of binary class.
 */
#pragma once

#include <algorithm>
#include <cstddef>  // IWYU pragma: keep
#include <cstring>
#include <initializer_list>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <string_view>
#include <vector>

#include "msgpack_light/details/basic_binary_buffer.h"

namespace msgpack_light {

namespace details {

/*!
 * \brief Calculate the size of an expanded buffer.
 *
 * \param[in] current_size Current size.
 * \param[in] additional_size Additional size.
 * \return Size of the expanded buffer.
 */
[[nodiscard]] inline std::size_t calculate_expanded_memory_buffer_size(
    std::size_t current_size, std::size_t additional_size) {
    std::size_t next_size = current_size;
    while (true) {
        next_size *= 2;
        if (next_size <= current_size) {
            // Overflow
            const std::size_t max_size =
                std::numeric_limits<std::size_t>::max();
            if (max_size - current_size >= additional_size) {
                return max_size;
            }
            throw std::bad_alloc();
        }
        if (next_size - current_size >= additional_size) {
            return next_size;
        }
    }
}

/*!
 * \brief Minimum capacity of the buffer in msgpack_light::binary class.
 */
constexpr std::size_t minimum_capacity_of_binary = 8U;

}  // namespace details

/*!
 * \brief Class of binary data.
 */
class binary {
public:
    /*!
     * \brief Constructor.
     *
     * Create empty data.
     */
    binary() : buffer_(details::minimum_capacity_of_binary), size_(0U) {}

    /*!
     * \brief Constructor.
     *
     * Create a buffer with uninitialized data.
     *
     * \param[in] size Size of the buffer.
     */
    explicit binary(std::size_t size)
        : buffer_(std::max(details::minimum_capacity_of_binary, size)),
          size_(size) {}

    /*!
     * \brief Constructor.
     *
     * \param[in] data Pointer to the data.
     * \param[in] size Size of the data.
     */
    binary(const unsigned char* data, std::size_t size) : binary(size) {
        std::memcpy(buffer_.data(), data, size);
    }

    /*!
     * \brief Constructor.
     *
     * \param[in] data Data.
     */
    explicit binary(const std::vector<unsigned char>& data)
        : binary(data.data(), data.size()) {}

    /*!
     * \brief Constructor.
     *
     * \param[in] data Data.
     */
    binary(std::initializer_list<unsigned char> data)
        : binary(data.begin(), data.size()) {}

    /*!
     * \brief Constructor.
     *
     * \note This function accepts hex expression of data written with
     * numeric characters from `0` to `9` and uppercase characters from `A` to
     * `F`.
     *
     * \warning Current implementation doesn't accept lowercase characters
     * from `a` to `f`.
     *
     * \param[in] data_string Hex expression of data.
     */
    explicit binary(std::string_view data_string)
        : binary(data_string.size() / 2U) {
        constexpr std::string_view hex_digits = "0123456789ABCDEF";
        unsigned int byte = 0U;
        bool is_first_digit = true;
        std::size_t current_byte_index = 0U;
        for (char digit : data_string) {
            const auto pos = hex_digits.find(digit);
            if (pos == std::string_view::npos) {
                throw std::invalid_argument("Invalid hex expression.");
            }
            if (is_first_digit) {
                byte = static_cast<unsigned int>(pos);
                is_first_digit = false;
            } else {
                byte <<= 4U;
                byte |= static_cast<unsigned int>(pos);
                buffer_.data()[current_byte_index] =
                    static_cast<unsigned char>(byte);
                ++current_byte_index;
                is_first_digit = true;
            }
        }
        if (!is_first_digit) {
            throw std::invalid_argument("Invalid hex expression.");
        }
    }

    /*!
     * \brief Change the size of this data.
     *
     * This function preserves the existing data.
     * Additional bytes will be left uninitialized.
     *
     * \param[in] size New size.
     */
    void resize(std::size_t size) {
        if (size > buffer_.size()) {
            buffer_.resize(size);
        }
        size_ = size;
    }

    /*!
     * \brief Change the size of the internal buffer.
     *
     * This function preserves the existing data and its size.
     *
     * \param[in] size New size.
     */
    void reserve(std::size_t size) {
        if (size > buffer_.size()) {
            buffer_.resize(size);
        }
    }

    /*!
     * \brief Append another binary data.
     *
     * \param[in] data Pointer to the appended data.
     * \param[in] size Size of the appended data.
     */
    void append(const unsigned char* data, std::size_t size) {
        const std::size_t current_size = size_;
        const std::size_t remaining_capacity = buffer_.size() - size_;
        if (remaining_capacity < size) {
            reserve(details::calculate_expanded_memory_buffer_size(
                buffer_.size(), size - remaining_capacity));
        }
        std::memcpy(buffer_.data() + current_size, data, size);
        size_ += size;
    }

    /*!
     * \brief Append another binary data.
     *
     * \param[in] other Another binary data to append.
     * \return This.
     */
    binary& operator+=(const binary& other) {
        append(other.data(), other.size());
        return *this;
    }

    /*!
     * \brief Access to a byte.
     *
     * \param[in] index Index of the byte.
     * \return Reference to the byte.
     */
    [[nodiscard]] unsigned char& operator[](std::size_t index) noexcept {
        return buffer_.data()[index];
    }

    /*!
     * \brief Access to a byte.
     *
     * \param[in] index Index of the byte.
     * \return Value of the byte.
     */
    [[nodiscard]] unsigned char operator[](std::size_t index) const noexcept {
        return buffer_.data()[index];
    }

    /*!
     * \brief Get the pointer to the data.
     *
     * \return Pointer to the data.
     */
    [[nodiscard]] unsigned char* data() noexcept { return buffer_.data(); }

    /*!
     * \brief Get the pointer to the data.
     *
     * \return Pointer to the data.
     */
    [[nodiscard]] const unsigned char* data() const noexcept {
        return buffer_.data();
    }

    /*!
     * \brief Get the size of the data.
     *
     * \return Size of the data.
     */
    [[nodiscard]] std::size_t size() const noexcept { return size_; }

    /*!
     * \brief Get the size of the internal buffer for data.
     *
     * \return Size of the internal buffer.
     */
    [[nodiscard]] std::size_t capacity() const noexcept {
        return buffer_.size();
    }

    /*!
     * \brief Compare with another object.
     *
     * \param[in] other Another object.
     * \retval true Two object are equal.
     * \retval false Two object are not equal.
     */
    [[nodiscard]] bool operator==(const binary& other) const noexcept {
        return size_ == other.size_ &&
            std::memcmp(buffer_.data(), other.buffer_.data(), size_) == 0;
    }

    /*!
     * \brief Compare with another object.
     *
     * \param[in] other Another object.
     * \retval true Two object are not equal.
     * \retval false Two object are equal.
     */
    [[nodiscard]] bool operator!=(const binary& other) const noexcept {
        return !operator==(other);
    }

private:
    //! Buffer.
    details::basic_binary_buffer buffer_;

    //! Size.
    std::size_t size_;
};

/*!
 * \brief Connect two binary data.
 *
 * \param[in] lhs Light-hand-side data.
 * \param[in] rhs Right-hand-side data.
 * \return Connected data.
 */
[[nodiscard]] inline binary operator+(const binary& lhs, const binary& rhs) {
    return binary(lhs) += rhs;
}

/*!
 * \brief Format a value to a stream.
 *
 * \param[in] stream Output stream.
 * \param[in] value Value.
 * \return Output stream.
 */
inline std::ostream& operator<<(std::ostream& stream, const binary& value) {
    constexpr std::string_view hex_digits = "0123456789ABCDEF";
    for (std::size_t i = 0; i < value.size(); ++i) {
        const unsigned int byte = value.data()[i];
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        stream << hex_digits[(byte >> 4U) & 0x0FU];
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        stream << hex_digits[byte & 0x0FU];
    }
    return stream;
}

}  // namespace msgpack_light
