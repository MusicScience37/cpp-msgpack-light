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
 * \brief Definition of classes of references to objects.
 */
#pragma once

#include "msgpack_light/details/allocator_wrapper.h"
#include "msgpack_light/details/object_base.h"
#include "msgpack_light/details/object_data.h"
#include "msgpack_light/details/object_ref_decl.h"  // IWYU pragma: keep

namespace msgpack_light {

/*!
 * \brief Class to access non-constant objects.
 *
 * \tparam Allocator Type of the allocator.
 *
 * \note Instances of this class can be created from
 * msgpack_light::mutable_array_ref,
 * msgpack_light::mutable_array_iterator,
 * msgpack_light::mutable_map_ref,
 * msgpack_light::mutable_map_iterator,
 * classes.
 *
 * \warning This class only holds pointers to data in msgpack_light::object
 * class, do not call functions in this class without msgpack_light::object
 * instances holding the data.
 */
template <typename Allocator>
class mutable_object_ref
    : public details::mutable_object_base<mutable_object_ref<Allocator>,
          Allocator> {
public:
    //! Type of the base class.
    using base_type =
        details::mutable_object_base<mutable_object_ref<Allocator>, Allocator>;

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
    mutable_object_ref(details::object_data& data,
        details::allocator_wrapper<Allocator>& allocator)
        : data_(&data), allocator_(&allocator) {}

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
    [[nodiscard]] details::allocator_wrapper<Allocator>& allocator() noexcept {
        return *allocator_;
    }

    //!\}

private:
    //! Data.
    details::object_data* data_;

    //! Allocator.
    details::allocator_wrapper<Allocator>* allocator_;
};

/*!
 * \brief Class to access constant objects.
 *
 * \note Instances of this class can be created from
 * msgpack_light::mutable_array_ref, msgpack_light::const_array_ref,
 * msgpack_light::mutable_array_iterator, msgpack_light::const_array_iterator,
 * msgpack_light::mutable_map_ref, msgpack_light::const_map_ref,
 * msgpack_light::mutable_map_iterator, msgpack_light::const_map_iterator
 * classes.
 *
 * \warning This class only holds pointers to data in msgpack_light::object
 * class, do not call functions in this class without msgpack_light::object
 * instances holding the data.
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
        return *data_;
    }

    //!\}

private:
    //! Data.
    const details::object_data* data_;
};

}  // namespace msgpack_light
