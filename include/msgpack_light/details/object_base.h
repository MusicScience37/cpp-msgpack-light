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
 * \brief Definition of base classes of objects.
 */
#pragma once

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string_view>

#include "msgpack_light/binary.h"
#include "msgpack_light/details/allocator_wrapper.h"
#include "msgpack_light/details/array_ref.h"
#include "msgpack_light/details/extension_ref.h"
#include "msgpack_light/details/map_ref.h"
#include "msgpack_light/details/object_data.h"
#include "msgpack_light/details/object_helper.h"
#include "msgpack_light/details/object_ref_decl.h"  // IWYU pragma: keep
#include "msgpack_light/object_data_type.h"

namespace msgpack_light::details {

/*!
 * \brief Base class of constant objects in MessagePack.
 *
 * \tparam Derived Type of the derived class.
 *
 * \warning This class cannot be instantiated directly. Instantiate
 * msgpack_light::object classes.
 */
template <typename Derived>
class const_object_base {
public:
    /*!
     * \name Access to data
     */
    //!\{

    /*!
     * \brief Get the type of this object.
     *
     * \return Type of this object.
     */
    [[nodiscard]] object_data_type type() const noexcept { return data().type; }

    /*!
     * \brief Get data as an unsigned integer.
     *
     * \return Value.
     */
    [[nodiscard]] std::uint64_t as_unsigned_integer() const {
        if (data().type != object_data_type::unsigned_integer) {
            throw std::runtime_error("This object is not an unsigned integer.");
        }
        return data().data.unsigned_integer_value;
    }

    /*!
     * \brief Get data as a signed integer.
     *
     * \return Value.
     */
    [[nodiscard]] std::int64_t as_signed_integer() const {
        if (data().type != object_data_type::signed_integer) {
            throw std::runtime_error("This object is not a signed integer.");
        }
        return data().data.signed_integer_value;
    }

    /*!
     * \brief Get data as a boolean.
     *
     * \return Value.
     */
    [[nodiscard]] bool as_boolean() const {
        if (data().type != object_data_type::boolean) {
            throw std::runtime_error("This object is not a boolean.");
        }
        return data().data.bool_value;
    }

    /*!
     * \brief Get data as a 32-bit floating-pointe number.
     *
     * \return Value.
     */
    [[nodiscard]] float as_float32() const {
        if (data().type != object_data_type::float32) {
            throw std::runtime_error(
                "This object is not a 32-bit floating-point number.");
        }
        return data().data.float_value;
    }

    /*!
     * \brief Get data as a 64-bit floating-pointe number.
     *
     * \return Value.
     */
    [[nodiscard]] double as_float64() const {
        if (data().type != object_data_type::float64) {
            throw std::runtime_error(
                "This object is not a 64-bit floating-point number.");
        }
        return data().data.double_value;
    }

    /*!
     * \brief Get data as a string.
     *
     * \return Value.
     */
    [[nodiscard]] std::string_view as_string() const {
        if (data().type != object_data_type::string) {
            throw std::runtime_error("This object is not a string.");
        }
        return std::string_view(
            data().data.string_value.data, data().data.string_value.size);
    }

    /*!
     * \brief Get data as a binary.
     *
     * \return Value.
     */
    [[nodiscard]] binary_view as_binary() const {
        if (data().type != object_data_type::binary) {
            throw std::runtime_error("This object is not a binary.");
        }
        return binary_view(
            data().data.binary_value.data, data().data.binary_value.size);
    }

    /*!
     * \brief Get data as an array.
     *
     * \return Value.
     */
    [[nodiscard]] const_array_ref as_array() const {
        if (data().type != object_data_type::array) {
            throw std::runtime_error("This object is not an array.");
        }
        return const_array_ref{data().data.array_value};
    }

    /*!
     * \brief Get data as a map.
     *
     * \return Value.
     */
    [[nodiscard]] const_map_ref as_map() const {
        if (data().type != object_data_type::map) {
            throw std::runtime_error("This object is not a map.");
        }
        return const_map_ref{data().data.map_value};
    }

    /*!
     * \brief Get data as an extension.
     *
     * \return Value.
     */
    [[nodiscard]] const_extension_ref as_extension() const {
        if (data().type != object_data_type::extension) {
            throw std::runtime_error("This object is not an extension.");
        }
        return const_extension_ref{data().data.extension_value};
    }

    //!\}

    /*!
     * \name Internal data
     *
     * \warning These functions are for internal implementation of this library.
     */
    //!\{

    /*!
     * \brief Get the internal data.
     *
     * \return Internal data.
     */
    [[nodiscard]] const details::object_data& data() const noexcept {
        return derived().data();
    }

    //!\}

protected:
    /*!
     * \brief Access the derived class.
     *
     * \return Reference to the derived class.
     */
    [[nodiscard]] const Derived& derived() const noexcept {
        return *static_cast<const Derived*>(this);
    }
};

/*!
 * \brief Base class of non-constant objects in MessagePack.
 *
 * \tparam Derived Type of the derived class.
 * \tparam Allocator Type of the allocator.
 *
 * \warning This class cannot be instantiated directly. Instantiate
 * msgpack_light::object classes.
 */
template <typename Derived, typename Allocator>
class mutable_object_base : public const_object_base<Derived> {
public:
    //! Type of the allocator.
    using allocator_type = Allocator;

    /*!
     * \name Set data
     */
    //!\{

    /*!
     * \brief Set this object to an unsigned integer.
     *
     * \param[in] value Value.
     */
    void set_unsigned_integer(std::uint64_t value) noexcept {
        clear();
        data().data.unsigned_integer_value = value;
        data().type = object_data_type::unsigned_integer;
    }

    /*!
     * \brief Set this object to a signed integer.
     *
     * \param[in] value Value.
     */
    void set_signed_integer(std::int64_t value) noexcept {
        clear();
        data().data.signed_integer_value = value;
        data().type = object_data_type::signed_integer;
    }

    /*!
     * \brief Set this object to a boolean value.
     *
     * \param[in] value Value.
     */
    void set_boolean(bool value) noexcept {
        clear();
        data().data.bool_value = value;
        data().type = object_data_type::boolean;
    }

    /*!
     * \brief Set this object to a 32-bit floating-point number.
     *
     * \param[in] value Value.
     */
    void set_float32(float value) noexcept {
        clear();
        data().data.float_value = value;
        data().type = object_data_type::float32;
    }

    /*!
     * \brief Set this object to a 64-bit floating-point number.
     *
     * \param[in] value Value.
     */
    void set_float64(double value) noexcept {
        clear();
        data().data.double_value = value;
        data().type = object_data_type::float64;
    }

    /*!
     * \brief Set this object to a string.
     *
     * \param[in] value Value.
     */
    void set_string(std::string_view value) {
        auto* ptr = allocator().allocate_char(value.size());
        std::memcpy(ptr, value.data(), value.size());
        clear();
        data().data.string_value.data = ptr;
        data().data.string_value.size = value.size();
        data().type = object_data_type::string;
    }

    /*!
     * \brief Set this object to a binary.
     *
     * \param[in] value Value.
     */
    void set_binary(binary_view value) {
        auto* ptr = allocator().allocate_unsigned_char(value.size());
        std::memcpy(ptr, value.data(), value.size());
        clear();
        data().data.binary_value.data = ptr;
        data().data.binary_value.size = value.size();
        data().type = object_data_type::binary;
    }

    /*!
     * \brief Set this object to an array.
     *
     * \param[in] size Number of elements.
     * \return Object to access the array.
     */
    mutable_array_ref<Allocator> set_array(std::size_t size = 0U) {
        clear();
        data().data.array_value.data = allocator().allocate_object_data(size);
        data().data.array_value.size = size;
        std::memset(
            data().data.array_value.data, 0, size * sizeof(object_data));
        data().type = object_data_type::array;
        return mutable_array_ref<Allocator>(
            data().data.array_value, allocator());
    }

    /*!
     * \brief Set this object to a map.
     *
     * \param[in] size Number of key-value pairs.
     * \return Object to access the map.
     */
    mutable_map_ref<Allocator> set_map(std::size_t size = 0U) {
        clear();
        data().data.map_value.data =
            allocator().allocate_key_value_pair_data(size);
        data().data.map_value.size = size;
        std::memset(
            data().data.map_value.data, 0, size * sizeof(key_value_pair_data));
        data().type = object_data_type::map;
        return mutable_map_ref<Allocator>(data().data.map_value, allocator());
    }

    /*!
     * \brief Set this object to a extension value.
     *
     * \param[in] type Type.
     * \param[in] value_data Data of the value.
     */
    void set_extension(std::int8_t type, binary_view value_data) {
        auto* ptr = allocator().allocate_unsigned_char(value_data.size());
        std::memcpy(ptr, value_data.data(), value_data.size());
        clear();
        data().data.extension_value.type = type;
        data().data.extension_value.data = ptr;
        data().data.extension_value.size = value_data.size();
        data().type = object_data_type::extension;
    }

    /*!
     * \brief Clear the data.
     */
    void clear() noexcept { clear_object_data(data(), allocator()); }

    //!\}

    using const_object_base<Derived>::as_array;
    using const_object_base<Derived>::as_map;

    /*!
     * \name Access to data
     */
    //!\{

    /*!
     * \brief Get data as an array.
     *
     * \return Value.
     */
    [[nodiscard]] mutable_array_ref<Allocator> as_array() {
        if (data().type != object_data_type::array) {
            throw std::runtime_error("This object is not an array.");
        }
        return mutable_array_ref<Allocator>(
            data().data.array_value, allocator());
    }

    /*!
     * \brief Get data as a map.
     *
     * \return Value.
     */
    [[nodiscard]] mutable_map_ref<Allocator> as_map() {
        if (data().type != object_data_type::map) {
            throw std::runtime_error("This object is not a map.");
        }
        return mutable_map_ref<Allocator>(data().data.map_value, allocator());
    }

    //!\}

    /*!
     * \name Internal data
     *
     * \warning These functions are for internal implementation of this library.
     */
    //!\{

    /*!
     * \brief Get the internal data.
     *
     * \return Internal data.
     */
    [[nodiscard]] details::object_data& data() noexcept {
        return derived().data();
    }

    /*!
     * \brief Get the internal data.
     *
     * \return Internal data.
     */
    [[nodiscard]] const details::object_data& data() const noexcept {
        return derived().data();
    }

    /*!
     * \brief Get the allocator.
     *
     * \return Allocator.
     */
    [[nodiscard]] allocator_wrapper<Allocator>& allocator() noexcept {
        return derived().allocator();
    }

    //!\}

protected:
    /*!
     * \brief Access the derived class.
     *
     * \return Reference to the derived class.
     */
    [[nodiscard]] Derived& derived() noexcept {
        return *static_cast<Derived*>(this);
    }

    /*!
     * \brief Access the derived class.
     *
     * \return Reference to the derived class.
     */
    [[nodiscard]] const Derived& derived() const noexcept {
        return *static_cast<const Derived*>(this);
    }
};

}  // namespace msgpack_light::details
