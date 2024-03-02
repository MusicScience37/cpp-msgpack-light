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
 * \brief Implementation of classes of references to arrays.
 */
#pragma once

#include <cstring>

#include "msgpack_light/details/array_ref.h"
#include "msgpack_light/details/object_data.h"
#include "msgpack_light/details/object_ref.h"  // IWYU pragma: keep

namespace msgpack_light {

/*
 * Definition of some members of mutable_array_ref.
 */

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

/*
 * Definition of some members of const_array_ref.
 */

inline typename const_array_ref::const_object_ref_type
const_array_ref::operator[](std::size_t index) const noexcept {
    return const_object_ref_type{data_->data[index]};
}

}  // namespace msgpack_light
