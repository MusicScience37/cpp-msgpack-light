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

#include <cstring>

#include "msgpack_light/details/allocator_wrapper.h"    // IWYU pragma: export
#include "msgpack_light/details/array_iterator.h"       // IWYU pragma: export
#include "msgpack_light/details/array_iterator_impl.h"  // IWYU pragma: export
#include "msgpack_light/details/array_ref.h"            // IWYU pragma: export
#include "msgpack_light/details/array_ref_impl.h"       // IWYU pragma: export
#include "msgpack_light/details/extension_ref.h"        // IWYU pragma: export
#include "msgpack_light/details/map_iterator.h"         // IWYU pragma: export
#include "msgpack_light/details/map_iterator_impl.h"    // IWYU pragma: export
#include "msgpack_light/details/map_ref.h"              // IWYU pragma: export
#include "msgpack_light/details/map_ref_impl.h"         // IWYU pragma: export
#include "msgpack_light/details/object_base.h"          // IWYU pragma: export
#include "msgpack_light/details/object_data.h"          // IWYU pragma: export
#include "msgpack_light/details/object_helper.h"        // IWYU pragma: export
#include "msgpack_light/details/object_ref.h"           // IWYU pragma: export
#include "msgpack_light/details/object_ref_decl.h"      // IWYU pragma: export
#include "msgpack_light/standard_allocator.h"

namespace msgpack_light {

/*!
 * \brief Class of objects in MessagePack.
 *
 * \tparam Allocator Type of the allocator.
 */
template <typename Allocator = standard_allocator>
class object
    : public details::mutable_object_base<object<Allocator>, Allocator> {
public:
    //! Type of the base class.
    using base_type =
        details::mutable_object_base<object<Allocator>, Allocator>;

#ifndef MSGPACK_LIGHT_DOCUMENTATION
    using base_type::clear;
    using typename base_type::allocator_type;
#endif

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
     * \param[in] other Another instance to copy from.
     */
    object(const object& other) : allocator_(other.allocator_) {
        details::copy_object_data(data_, other.data_, allocator_);
    }

    /*!
     * \brief Move constructor.
     *
     * \param[in,out] other Another instance to move from.
     */
    object(object&& other) noexcept
        : data_(other.data_), allocator_(std::move(other.allocator_)) {
        std::memset(&other.data_, 0, sizeof(other.data_));
    }

    /*!
     * \brief Copy assignment operator.
     *
     * \param[in] other Another instance to copy from.
     * \return This instance after copy.
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
     * \param[in,out] other Another instance to move from.
     * \return This instance after move.
     */
    object& operator=(object&& other) noexcept {
        swap(other);
        return *this;
    }

    /*!
     * \brief Swap this instance with another instance.
     *
     * \param[in,out] other Another instance to move with.
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
     *
     * \warning These functions are for internal implementation of this library.
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
    [[nodiscard]] details::allocator_wrapper<Allocator>& allocator() noexcept {
        return allocator_;
    }

    //!\}

private:
    //! Data.
    details::object_data data_{};

    //! Allocator.
    details::allocator_wrapper<Allocator> allocator_;
};

}  // namespace msgpack_light
