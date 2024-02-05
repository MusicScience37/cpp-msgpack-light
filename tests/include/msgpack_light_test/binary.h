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
#include <initializer_list>
#include <iterator>
#include <ostream>
#include <string_view>
#include <vector>

#include <fmt/format.h>

namespace msgpack_light_test {

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
    binary() = default;

    /*!
     * \brief Constructor.
     *
     * \param[in] data Data.
     */
    explicit binary(std::vector<unsigned char> data) : data_(std::move(data)) {}

    /*!
     * \brief Constructor.
     *
     * \param[in] data Data.
     */
    binary(std::initializer_list<unsigned char> data) : data_(data) {}

    /*!
     * \brief Constructor.
     *
     * \param[in] data_string Hex string of data.
     */
    explicit binary(std::string_view data_string) {
        constexpr std::string_view hex_digits = "0123456789ABCDEF";
        unsigned int byte = 0U;
        bool is_first_digit = true;
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
                data_.push_back(static_cast<unsigned char>(byte));
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
    binary(const unsigned char* data, std::size_t size)
        : data_(data, data + size) {}

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
        return data_ == other.data_;
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
        data_.insert(data_.end(), other.data_.begin(), other.data_.end());
        return *this;
    }

private:
    //! Data.
    std::vector<unsigned char> data_;
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

}  // namespace msgpack_light_test

namespace fmt {

/*!
 * \brief Specialization of fmt::formatter for msgpack_light_test::binary.
 */
template <>
struct formatter<msgpack_light_test::binary> {
    /*!
     * \brief Parse a format.
     *
     * \param[in] context Context in fmt library.
     * \return Iterator after parsing.
     */
    constexpr auto
    parse(  // NOLINT(readability-convert-member-functions-to-static): Interface of an external library.
        format_parse_context& context) -> format_parse_context::iterator {
        return context.end();
    }

    /*!
     * \brief Format a value.
     *
     * \param[in] value Value.
     * \param[in] context Context in fmt library.
     * \return Iterator after formatting.
     */
    auto
    format(  // NOLINT(readability-convert-member-functions-to-static): Interface of an external library.
        const msgpack_light_test::binary& value, format_context& context) const
        -> format_context::iterator {
        return fmt::format_to(context.out(), "{:02X}",
            fmt::join(value.data(), value.data() + value.size(), ""));
    }
};

}  // namespace fmt

namespace msgpack_light_test {

/*!
 * \brief Format a value to a stream.
 *
 * \param[in] stream Output stream.
 * \param[in] value Value.
 * \return Output stream.
 */
inline std::ostream& operator<<(std::ostream& stream, const binary& value) {
    fmt::format_to(std::ostreambuf_iterator<char>(stream), "{}", value);
    return stream;
}

}  // namespace msgpack_light_test
