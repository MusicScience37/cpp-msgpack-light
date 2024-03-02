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
 * \brief Implementation of classes of iterators of arrays.
 */
#pragma once

#include "msgpack_light/details/array_iterator.h"
#include "msgpack_light/details/object_ref.h"  // IWYU pragma: keep

namespace msgpack_light {

/*
 * Definition of some members of mutable_array_iterator.
 */

template <typename Allocator>
inline mutable_object_ref<Allocator>
mutable_array_iterator<Allocator>::operator*() const noexcept {
    return mutable_object_ref<Allocator>(*pointer_, *allocator_);
}

/*!
 * \brief Compare two iterators.
 *
 * \tparam Allocator Type of the allocator.
 * \param[in] lhs Light-hand-side object.
 * \param[in] rhs Right-hand-side object.
 * \retval true Two objects are equal.
 * \retval false Two objects are not equal.
 */
template <typename Allocator>
[[nodiscard]] inline bool operator==(mutable_array_iterator<Allocator> lhs,
    mutable_array_iterator<Allocator> rhs) noexcept {
    return &(*lhs).data() == &(*rhs).data();
}

/*!
 * \brief Compare two iterators.
 *
 * \tparam Allocator Type of the allocator.
 * \param[in] lhs Light-hand-side object.
 * \param[in] rhs Right-hand-side object.
 * \retval true Two objects are not equal.
 * \retval false Two objects are equal.
 */
template <typename Allocator>
[[nodiscard]] inline bool operator!=(mutable_array_iterator<Allocator> lhs,
    mutable_array_iterator<Allocator> rhs) noexcept {
    return !(lhs == rhs);
}

/*
 * Definition of some members of const_array_iterator.
 */

inline const_object_ref const_array_iterator::operator*() const noexcept {
    return const_object_ref(*pointer_);
}

/*!
 * \brief Compare two iterators.
 *
 * \param[in] lhs Light-hand-side object.
 * \param[in] rhs Right-hand-side object.
 * \retval true Two objects are equal.
 * \retval false Two objects are not equal.
 */
[[nodiscard]] inline bool operator==(
    const_array_iterator lhs, const_array_iterator rhs) noexcept {
    return &(*lhs).data() == &(*rhs).data();
}

/*!
 * \brief Compare two iterators.
 *
 * \param[in] lhs Light-hand-side object.
 * \param[in] rhs Right-hand-side object.
 * \retval true Two objects are not equal.
 * \retval false Two objects are equal.
 */
[[nodiscard]] inline bool operator!=(
    const_array_iterator lhs, const_array_iterator rhs) noexcept {
    return !(lhs == rhs);
}

}  // namespace msgpack_light
