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

#include <cstdint>
#include <stdexcept>
#include <utility>

#include "msgpack_light/details/object_data.h"
#include "msgpack_light/object_data_type.h"
#include "msgpack_light/standard_allocator.h"

namespace msgpack_light {

namespace details {

/*!
 * \brief Base class of objects in MessagePack.
 *
 * \tparam Derived Type of the derived class.
 * \tparam Allocator Type of the allocator.
 */
template <typename Derived, typename Allocator>
class object_base {
public:
    //! Type of the allocator.
    using allocator_type = Allocator;

    /*!
     * \brief Get the type of this object.
     *
     * \return Type of this object.
     */
    [[nodiscard]] object_data_type type() const noexcept { return data().type; }

    /*!
     * \brief Set this object to an unsigned integer.
     *
     * \param[in] value Value.
     */
    void set_unsigned_integer(std::uint64_t value) {
        clear();
        data().data.unsigned_integer_value = value;
        data().type = object_data_type::unsigned_integer;
    }

    /*!
     * \brief Clear the data.
     */
    void clear() { data().type = object_data_type::nil; }

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
 * \brief Class of objects in MessagePack.
 *
 * \tparam Allocator Type of the allocator.
 */
template <typename Allocator = standard_allocator>
class object : public details::object_base<object<Allocator>, Allocator> {
public:
    //! Type of the base class.
    using base_type = details::object_base<object<Allocator>, Allocator>;

    using typename base_type::allocator_type;

    /*!
     * \brief Constructor.
     *
     * \param[in] allocator Allocator.
     */
    explicit object(allocator_type allocator = allocator_type())
        : allocator_(std::move(allocator)) {}

    // TODO implement copy and move.
    object(const object&) = delete;
    object(object&&) = delete;
    object& operator=(const object&) = delete;
    object& operator=(object&&) = delete;

    /*!
     * \brief Destructor.
     */
    ~object() = default;

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

private:
    //! Data.
    details::object_data data_{};

    //! Allocator.
    allocator_type allocator_;
};

}  // namespace msgpack_light
