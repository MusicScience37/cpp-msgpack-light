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

#include <cstddef>
#include <cstring>
#include <initializer_list>
#include <ostream>
#include <string_view>
#include <vector>

#include "msgpack_light/details/basic_binary_buffer.h"

namespace msgpack_light {

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
    binary() : data_(0U) {}

    /*!
     * \brief Constructor.
     *
     * \param[in] data Data.
     */
    explicit binary(const std::vector<unsigned char>& data)
        : data_(data.size()) {
        std::memcpy(data_.data(), data.data(), data.size());
    }

    /*!
     * \brief Constructor.
     *
     * \param[in] data Data.
     */
    binary(std::initializer_list<unsigned char> data) : data_(data.size()) {
        std::memcpy(data_.data(), data.begin(), data.size());
    }

    /*!
     * \brief Constructor.
     *
     * \param[in] data_string Hex string of data.
     */
    explicit binary(std::string_view data_string)
        : data_(data_string.size() / 2U) {
        constexpr std::string_view hex_digits = "0123456789ABCDEF";
        unsigned int byte = 0U;
        bool is_first_digit = true;
        std::size_t current_byte_index = 0U;
        for (char digit : data_string) {
            const auto pos = hex_digits.find(digit);
            if (pos == std::string_view::npos) {
                throw std::invalid_argument("Invalid hex string.");
            }
            if (is_first_digit) {
                byte = static_cast<unsigned int>(pos);
                is_first_digit = false;
            } else {
                byte <<= 4U;
                byte |= static_cast<unsigned int>(pos);
                data_.data()[current_byte_index] =
                    static_cast<unsigned char>(byte);
                ++current_byte_index;
                is_first_digit = true;
            }
        }
    }

    /*!
     * \brief Constructor.
     *
     * \param[in] data Pointer to the data.
     * \param[in] size Size of the data.
     */
    binary(const unsigned char* data, std::size_t size) : data_(size) {
        std::memcpy(data_.data(), data, size);
    }

    /*!
     * \brief Get the pointer to the data.
     *
     * \return Pointer to the data.
     */
    [[nodiscard]] const unsigned char* data() const noexcept {
        return data_.data();
    }

    /*!
     * \brief Get the size of the data.
     *
     * \return Size of the data.
     */
    [[nodiscard]] std::size_t size() const noexcept { return data_.size(); }

    /*!
     * \brief Compare with another object.
     *
     * \param[in] other Another object.
     * \retval true Two object are equal.
     * \retval false Two object are not equal.
     */
    [[nodiscard]] bool operator==(const binary& other) const noexcept {
        return data_.size() == other.data_.size() &&
            std::memcmp(data_.data(), other.data_.data(), data_.size()) == 0;
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

    /*!
     * \brief Append another binary data.
     *
     * \param[in] other Another binary data to append.
     * \return This.
     */
    binary& operator+=(const binary& other) {
        const std::size_t current_size = data_.size();
        data_.resize(current_size + other.data_.size());
        std::memcpy(data_.data() + current_size, other.data_.data(),
            other.data_.size());
        return *this;
    }

private:
    //! Data.
    details::basic_binary_buffer data_;
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
