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
 * \brief Definition of classes of iterators of arrays.
 */
#pragma once

#include <cstddef>

#include "msgpack_light/details/allocator_wrapper.h"
#include "msgpack_light/details/object_data.h"
#include "msgpack_light/details/object_ref_decl.h"
#include "msgpack_light/standard_allocator.h"

namespace msgpack_light {

/*!
 * \brief Class of iterators of arrays to access non-constant objects.
 *
 * \tparam Allocator Type of the allocator.
 *
 * \note Instances of this class can be created from
 * msgpack_light::mutable_array_ref class.
 *
 * \warning This class only holds pointers to data in msgpack_light::object
 * class, do not call functions in this class without msgpack_light::object
 * instances holding the data.
 */
template <typename Allocator = standard_allocator>
class mutable_array_iterator {
public:
    //! Type of differences.
    using difference_type = std::ptrdiff_t;

    //! Type of values.
    using value_type = mutable_object_ref<Allocator>;

    //! Type of references.
    using reference = mutable_object_ref<Allocator>;

    /*!
     * \brief Constructor.
     */
    mutable_array_iterator() noexcept
        : pointer_(nullptr), allocator_(nullptr) {}

    /*!
     * \brief Constructor.
     *
     * \param[in] pointer Pointer to the current data.
     * \param[in] allocator Allocator.
     */
    mutable_array_iterator(details::object_data* pointer,
        details::allocator_wrapper<Allocator>* allocator) noexcept
        : pointer_(pointer), allocator_(allocator) {}

    /*!
     * \brief Dereference this iterator.
     *
     * \return Reference to the current object.
     */
    mutable_object_ref<Allocator> operator*() const noexcept;

    /*!
     * \brief Increment this iterator.
     *
     * \return This iterator.
     */
    mutable_array_iterator& operator++() noexcept {
        ++pointer_;
        return *this;
    }

    /*!
     * \brief Increment this iterator.
     *
     * \return This iterator.
     */
    const mutable_array_iterator  // NOLINT(readability-const-return-type)
    operator++(int) noexcept {
        mutable_array_iterator copy{*this};
        ++(*this);
        return copy;
    }

private:
    //! Pointer to the current data.
    details::object_data* pointer_;

    //! Allocator.
    details::allocator_wrapper<Allocator>* allocator_;
};

/*!
 * \brief Class of iterators of arrays to access constant objects.
 *
 * \note Instances of this class can be created from
 * msgpack_light::const_array_ref, msgpack_light::mutable_array_ref classes.
 *
 * \warning This class only holds pointers to data in msgpack_light::object
 * class, do not call functions in this class without msgpack_light::object
 * instances holding the data.
 */
class const_array_iterator {
public:
    //! Type of differences.
    using difference_type = std::ptrdiff_t;

    //! Type of values.
    using value_type = const_object_ref;

    //! Type of references.
    using reference = const_object_ref;

    /*!
     * \brief Constructor.
     */
    const_array_iterator() noexcept : pointer_(nullptr) {}

    /*!
     * \brief Constructor.
     *
     * \param[in] pointer Pointer to the current data.
     */
    explicit const_array_iterator(const details::object_data* pointer) noexcept
        : pointer_(pointer) {}

    /*!
     * \brief Dereference this iterator.
     *
     * \return Reference to the current object.
     */
    const_object_ref operator*() const noexcept;

    /*!
     * \brief Increment this iterator.
     *
     * \return This iterator.
     */
    const_array_iterator& operator++() noexcept {
        ++pointer_;
        return *this;
    }

    /*!
     * \brief Increment this iterator.
     *
     * \return This iterator.
     */
    const const_array_iterator  // NOLINT(readability-const-return-type)
    operator++(int) noexcept {
        const_array_iterator copy{*this};
        ++(*this);
        return copy;
    }

private:
    //! Pointer to the current data.
    const details::object_data* pointer_;
};

}  // namespace msgpack_light
