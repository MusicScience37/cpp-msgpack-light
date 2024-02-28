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
 * \brief Definition of object class.
 */
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string_view>
#include <utility>

#include "msgpack_light/details/object_data.h"
#include "msgpack_light/object_data_type.h"
#include "msgpack_light/standard_allocator.h"

namespace msgpack_light {

namespace details {

/*!
 * \brief Clear data.
 *
 * \tparam Allocator Type of the allocator.
 * \param[in,out] data Data.
 * \param[in] allocator Allocator.
 */
template <typename Allocator>
inline void clear_object_data(object_data& data, Allocator& allocator) {
    switch (data.type) {
    case object_data_type::string:
        allocator.deallocate(data.data.string_value.data);
        break;
    case object_data_type::array:
        for (std::size_t i = 0; i < data.data.array_value.size; ++i) {
            clear_object_data(data.data.array_value.data[i], allocator);
        }
        allocator.deallocate(data.data.array_value.data);
        break;
    default:
        break;
    };
    data.type = object_data_type::nil;
}

/*!
 * \brief Copy data.
 *
 * \tparam Allocator Type of the allocator.
 * \param[out] to Object to copy to.
 * \param[in] from Object to copy from.
 * \param[in] allocator Allocator.
 */
template <typename Allocator>
inline void copy_object_data(
    object_data& to, const object_data& from, Allocator& allocator) {
    switch (from.type) {
    case object_data_type::string:
        to.data.string_value.data =
            static_cast<char*>(allocator.allocate(from.data.string_value.size));
        std::memcpy(to.data.string_value.data, from.data.string_value.data,
            from.data.string_value.size);
        to.data.string_value.size = from.data.string_value.size;
        to.type = object_data_type::string;
        break;
    case object_data_type::array:
        to.data.array_value.data = static_cast<object_data*>(allocator.allocate(
            from.data.array_value.size * sizeof(object_data)));
        to.data.array_value.size = from.data.array_value.size;
        for (std::size_t i = 0; i < from.data.array_value.size; ++i) {
            copy_object_data(to.data.array_value.data[i],
                from.data.array_value.data[i], allocator);
        }
        to.type = object_data_type::array;
        break;
    default:
        to = from;
        break;
    };
}

}  // namespace details

template <typename Allocator = standard_allocator>
class mutable_object_ref;

class const_object_ref;

/*!
 * \brief Class of access constant arrays.
 *
 * \tparam Allocator
 */
class const_array_ref {
public:
    //! Type to access constant objects.
    using const_object_ref_type = const_object_ref;

    /*!
     * \brief Constructor.
     *
     * \param[in] data Data.
     */
    explicit const_array_ref(const details::array_data& data) : data_(&data) {}

    /*!
     * \brief Get the size.
     *
     * \return Size.
     */
    [[nodiscard]] std::size_t size() const noexcept { return data_->size; }

    /*!
     * \brief Get an object.
     *
     * \param[in] index Index of the object.
     * \return Object.
     */
    [[nodiscard]] const_object_ref_type operator[](
        std::size_t index) const noexcept;

private:
    //! Data.
    const details::array_data* data_;
};

/*!
 * \brief Class of access non-constant arrays.
 *
 * \tparam Allocator
 */
template <typename Allocator = standard_allocator>
class mutable_array_ref {
public:
    //! Type of the allocator.
    using allocator_type = Allocator;

    //! Type to access non-constant objects.
    using mutable_object_ref_type = mutable_object_ref<Allocator>;

    //! Type to access constant objects.
    using const_object_ref_type = const_object_ref;

    /*!
     * \brief Constructor.
     *
     * \param[in] data Data.
     * \param[in] allocator Allocator.
     */
    mutable_array_ref(details::array_data& data, allocator_type& allocator)
        : data_(&data), allocator_(&allocator) {}

    /*!
     * \brief Change the size.
     *
     * \param[in] size Size.
     */
    void resize(std::size_t size) {
        if (size < data_->size) {
            for (std::size_t i = size; i < data_->size; ++i) {
                details::clear_object_data(data_->data[i], *allocator_);
            }
        }
        data_->data = static_cast<details::object_data*>(allocator_->reallocate(
            data_->data, size * sizeof(details::object_data)));
        if (size > data_->size) {
            std::memset(data_->data + data_->size, 0,
                (size - data_->size) * sizeof(details::object_data));
        }
        data_->size = size;
    }

    /*!
     * \brief Get the size.
     *
     * \return Size.
     */
    [[nodiscard]] std::size_t size() const noexcept { return data_->size; }

    /*!
     * \brief Get an object.
     *
     * \param[in] index Index of the object.
     * \return Object.
     */
    [[nodiscard]] mutable_object_ref_type operator[](
        std::size_t index) noexcept;

    /*!
     * \brief Get an object.
     *
     * \param[in] index Index of the object.
     * \return Object.
     */
    [[nodiscard]] const_object_ref_type operator[](
        std::size_t index) const noexcept;

private:
    //! Data.
    details::array_data* data_;

    //! Allocator.
    allocator_type* allocator_;
};

namespace details {

/*!
 * \brief Base class of objects in MessagePack.
 *
 * \tparam Derived Type of the derived class.
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
    [[nodiscard]] float as_float64() const {
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

    //!\}

    /*!
     * \name Internal data
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
 * \brief Base class of objects in MessagePack.
 *
 * \tparam Derived Type of the derived class.
 * \tparam Allocator Type of the allocator.
 */
template <typename Derived, typename Allocator>
class object_base : public const_object_base<Derived> {
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
        auto* ptr = static_cast<char*>(allocator().allocate(value.size()));
        std::memcpy(ptr, value.data(), value.size());
        clear();
        data().data.string_value.data = ptr;
        data().data.string_value.size = value.size();
        data().type = object_data_type::string;
    }

    /*!
     * \brief Set this object to an array.
     *
     * \return Object to access the array.
     */
    mutable_array_ref<Allocator> set_array() {
        clear();
        data().data.array_value.data =
            static_cast<object_data*>(allocator().allocate(0U));
        data().data.array_value.size = 0U;
        data().type = object_data_type::array;
        return mutable_array_ref<Allocator>(
            data().data.array_value, allocator());
    }

    /*!
     * \brief Clear the data.
     */
    void clear() noexcept { clear_object_data(data(), allocator()); }

    //!\}

    /*!
     * \name Access to data
     */
    //!\{

    using const_object_base<Derived>::as_array;

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

    //!\}

    /*!
     * \name Internal data
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
    [[nodiscard]] allocator_type& allocator() noexcept {
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

}  // namespace details

/*!
 * \brief Class to access non-constant objects.
 *
 * \tparam Allocator Type of the allocator.
 */
template <typename Allocator>
class mutable_object_ref
    : public details::object_base<mutable_object_ref<Allocator>, Allocator> {
public:
    //! Type of the base class.
    using base_type =
        details::object_base<mutable_object_ref<Allocator>, Allocator>;

    using typename base_type::allocator_type;

    /*!
     * \name Initialization and finalization
     */
    //!\{

    /*!
     * \brief Constructor.
     *
     * \param[in] data Data.
     * \param[in] allocator Allocator.
     */
    mutable_object_ref(details::object_data& data, allocator_type& allocator)
        : data_(&data), allocator_(&allocator) {}

    //!\}

    /*!
     * \name Internal data
     */
    //!\{

    /*!
     * \brief Get the internal data.
     *
     * \return Internal data.
     */
    [[nodiscard]] details::object_data& data() noexcept { return *data_; }

    /*!
     * \brief Get the internal data.
     *
     * \return Internal data.
     */
    [[nodiscard]] const details::object_data& data() const noexcept {
        return *data_;
    }

    /*!
     * \brief Get the allocator.
     *
     * \return Allocator.
     */
    [[nodiscard]] allocator_type& allocator() noexcept { return *allocator_; }

    //!\}

private:
    //! Data.
    details::object_data* data_;

    //! Allocator.
    allocator_type* allocator_;
};

/*!
 * \brief Class to access constant objects.
 */
class const_object_ref : public details::const_object_base<const_object_ref> {
public:
    //! Type of the base class.
    using base_type = details::const_object_base<const_object_ref>;

    /*!
     * \name Initialization and finalization
     */
    //!\{

    /*!
     * \brief Constructor.
     *
     * \param[in] data Data.
     */
    explicit const_object_ref(const details::object_data& data)
        : data_(&data) {}

    //!\}

    /*!
     * \name Internal data
     */
    //!\{

    /*!
     * \brief Get the internal data.
     *
     * \return Internal data.
     */
    [[nodiscard]] const details::object_data& data() const noexcept {
        return *data_;
    }

    //!\}

private:
    //! Data.
    const details::object_data* data_;
};

template <typename Allocator>
inline typename mutable_array_ref<Allocator>::mutable_object_ref_type
mutable_array_ref<Allocator>::operator[](std::size_t index) noexcept {
    return mutable_object_ref_type(data_->data[index], *allocator_);
}

template <typename Allocator>
inline typename mutable_array_ref<Allocator>::const_object_ref_type
mutable_array_ref<Allocator>::operator[](std::size_t index) const noexcept {
    return const_object_ref_type{data_->data[index]};
}

inline typename const_array_ref::const_object_ref_type
const_array_ref::operator[](std::size_t index) const noexcept {
    return const_object_ref_type{data_->data[index]};
}

/*!
 * \brief Class of objects in MessagePack.
 *
 * \tparam Allocator Type of the allocator.
 */
template <typename Allocator = standard_allocator>
class object : public details::object_base<object<Allocator>, Allocator> {
public:
    //! Type of the base class.
    using base_type = details::object_base<object<Allocator>, Allocator>;

    using base_type::clear;
    using typename base_type::allocator_type;

    /*!
     * \name Initialization and finalization
     */
    //!\{

    /*!
     * \brief Constructor.
     *
     * \param[in] allocator Allocator.
     */
    explicit object(allocator_type allocator = allocator_type())
        : allocator_(std::move(allocator)) {}

    /*!
     * \brief Copy constructor.
     *
     * \param[in] other Another object to copy from.
     */
    object(const object& other) : allocator_(other.allocator_) {
        details::copy_object_data(data_, other.data_, allocator_);
    }

    /*!
     * \brief Move constructor.
     *
     * \param[in,out] other Another object to move from.
     */
    object(object&& other) noexcept
        : data_(other.data_), allocator_(std::move(other.allocator_)) {
        std::memset(&other.data_, 0, sizeof(other.data_));
    }

    /*!
     * \brief Copy assignment operator.
     *
     * \param[in] other Another object to copy from.
     * \return This object after copy.
     */
    object& operator=(const object& other) {
        if (this == &other) {
            return *this;
        }
        clear();
        details::copy_object_data(data_, other.data_, allocator_);
        return *this;
    }

    /*!
     * \brief Move assignment operator.
     *
     * \param[in,out] other Another object to move from.
     * \return This object after move.
     */
    object& operator=(object&& other) noexcept {
        swap(other);
        return *this;
    }

    /*!
     * \brief Swap this object with another object.
     *
     * \param[in,out] other Another object to move with.
     */
    void swap(object& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(allocator_, other.allocator_);
    }

    /*!
     * \brief Destructor.
     */
    ~object() { clear(); }

    //!\}

    /*!
     * \name Internal data
     */
    //!\{

    /*!
     * \brief Get the internal data.
     *
     * \return Internal data.
     */
    [[nodiscard]] details::object_data& data() noexcept { return data_; }

    /*!
     * \brief Get the internal data.
     *
     * \return Internal data.
     */
    [[nodiscard]] const details::object_data& data() const noexcept {
        return data_;
    }

    /*!
     * \brief Get the allocator.
     *
     * \return Allocator.
     */
    [[nodiscard]] allocator_type& allocator() noexcept { return allocator_; }

    //!\}

private:
    //! Data.
    details::object_data data_{};

    //! Allocator.
    allocator_type allocator_;
};

}  // namespace msgpack_light
