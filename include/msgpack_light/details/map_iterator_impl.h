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

#include <utility>

#include "msgpack_light/details/map_iterator.h"
#include "msgpack_light/details/object_data.h"
#include "msgpack_light/details/object_ref.h"  // IWYU pragma: keep

namespace msgpack_light {

/*
 * Definition of some members of mutable_map_iterator.
 */

template <typename Allocator>
inline std::pair<mutable_object_ref<Allocator>, mutable_object_ref<Allocator>>
mutable_map_iterator<Allocator>::operator*() const noexcept {
    return {mutable_object_ref<Allocator>(pointer_->key, *allocator_),
        mutable_object_ref<Allocator>(pointer_->value, *allocator_)};
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
[[nodiscard]] inline bool operator==(mutable_map_iterator<Allocator> lhs,
    mutable_map_iterator<Allocator> rhs) noexcept {
    return &(*lhs).first.data() == &(*rhs).first.data();
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
[[nodiscard]] inline bool operator!=(mutable_map_iterator<Allocator> lhs,
    mutable_map_iterator<Allocator> rhs) noexcept {
    return !(lhs == rhs);
}

/*
 * Definition of some members of const_map_iterator.
 */

inline std::pair<const_object_ref, const_object_ref>
const_map_iterator::operator*() const noexcept {
    return {const_object_ref{pointer_->key}, const_object_ref{pointer_->value}};
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
    const_map_iterator lhs, const_map_iterator rhs) noexcept {
    return &(*lhs).first.data() == &(*rhs).first.data();
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
    const_map_iterator lhs, const_map_iterator rhs) noexcept {
    return !(lhs == rhs);
}

}  // namespace msgpack_light
