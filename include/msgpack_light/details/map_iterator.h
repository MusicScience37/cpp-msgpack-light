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
 * \brief Definition of classes of iterators of maps.
 */
#pragma once

#include <cstddef>
#include <utility>

#include "msgpack_light/details/allocator_wrapper.h"
#include "msgpack_light/details/object_data.h"
#include "msgpack_light/details/object_ref_decl.h"
#include "msgpack_light/standard_allocator.h"

namespace msgpack_light {

/*!
 * \brief Class of iterators of maps to access non-constant objects.
 *
 * \tparam Allocator Type of the allocator.
 */
template <typename Allocator = standard_allocator>
class mutable_map_iterator {
public:
    //! Type of differences.
    using difference_type = std::ptrdiff_t;

    //! Type of values.
    using value_type =
        std::pair<mutable_object_ref<Allocator>, mutable_object_ref<Allocator>>;

    //! Type of references.
    using reference = value_type;

    /*!
     * \brief Constructor.
     */
    mutable_map_iterator() noexcept : pointer_(nullptr), allocator_(nullptr) {}

    /*!
     * \brief Constructor.
     *
     * \param[in] pointer Pointer to the current data.
     * \param[in] allocator Allocator.
     */
    mutable_map_iterator(details::key_value_pair_data* pointer,
        details::allocator_wrapper<Allocator>* allocator) noexcept
        : pointer_(pointer), allocator_(allocator) {}

    /*!
     * \brief Dereference this iterator.
     *
     * \return References to the current key and value.
     */
    std::pair<mutable_object_ref<Allocator>, mutable_object_ref<Allocator>>
    operator*() const noexcept;

    /*!
     * \brief Increment this iterator.
     *
     * \return This iterator.
     */
    mutable_map_iterator& operator++() noexcept {
        ++pointer_;
        return *this;
    }

    /*!
     * \brief Increment this iterator.
     *
     * \return This iterator.
     */
    const mutable_map_iterator  // NOLINT(readability-const-return-type)
    operator++(int) noexcept {
        mutable_map_iterator copy{*this};
        ++(*this);
        return copy;
    }

private:
    //! Pointer to the current data.
    details::key_value_pair_data* pointer_;

    //! Allocator.
    details::allocator_wrapper<Allocator>* allocator_;
};

/*!
 * \brief Class of iterators of maps to access constant objects.
 */
class const_map_iterator {
public:
    //! Type of differences.
    using difference_type = std::ptrdiff_t;

    //! Type of values.
    using value_type = std::pair<const_object_ref, const_object_ref>;

    //! Type of references.
    using reference = value_type;

    /*!
     * \brief Constructor.
     */
    const_map_iterator() noexcept : pointer_(nullptr) {}

    /*!
     * \brief Constructor.
     *
     * \param[in] pointer Pointer to the current data.
     */
    explicit const_map_iterator(
        const details::key_value_pair_data* pointer) noexcept
        : pointer_(pointer) {}

    /*!
     * \brief Dereference this iterator.
     *
     * \return References to the current key and value.
     */
    std::pair<const_object_ref, const_object_ref> operator*() const noexcept;

    /*!
     * \brief Increment this iterator.
     *
     * \return This iterator.
     */
    const_map_iterator& operator++() noexcept {
        ++pointer_;
        return *this;
    }

    /*!
     * \brief Increment this iterator.
     *
     * \return This iterator.
     */
    const const_map_iterator  // NOLINT(readability-const-return-type)
    operator++(int) noexcept {
        const_map_iterator copy{*this};
        ++(*this);
        return copy;
    }

private:
    //! Pointer to the current data.
    const details::key_value_pair_data* pointer_;
};

}  // namespace msgpack_light
