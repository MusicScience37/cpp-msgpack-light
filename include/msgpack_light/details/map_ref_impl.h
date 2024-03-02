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
 * \brief Implementation of classes of references of maps.
 */
#pragma once

#include <cstring>

#include "msgpack_light/details/map_ref.h"
#include "msgpack_light/details/object_data.h"
#include "msgpack_light/details/object_ref.h"  // IWYU pragma: keep

namespace msgpack_light {

/*
 * Definition of some members of mutable_map_ref.
 */

template <typename Allocator>
inline typename mutable_map_ref<Allocator>::mutable_object_ref_type
mutable_map_ref<Allocator>::key(std::size_t index) noexcept {
    return mutable_object_ref_type(data_->data[index].key, *allocator_);
}

template <typename Allocator>
inline typename mutable_map_ref<Allocator>::const_object_ref_type
mutable_map_ref<Allocator>::key(std::size_t index) const noexcept {
    return const_object_ref_type{data_->data[index].key};
}

template <typename Allocator>
inline typename mutable_map_ref<Allocator>::mutable_object_ref_type
mutable_map_ref<Allocator>::value(std::size_t index) noexcept {
    return mutable_object_ref_type(data_->data[index].value, *allocator_);
}

template <typename Allocator>
inline typename mutable_map_ref<Allocator>::const_object_ref_type
mutable_map_ref<Allocator>::value(std::size_t index) const noexcept {
    return const_object_ref_type{data_->data[index].value};
}

/*
 * Definition of some members of const_map_ref.
 */

inline typename const_map_ref::const_object_ref_type const_map_ref::key(
    std::size_t index) const noexcept {
    return const_object_ref_type{data_->data[index].key};
}

inline typename const_map_ref::const_object_ref_type const_map_ref::value(
    std::size_t index) const noexcept {
    return const_object_ref_type{data_->data[index].value};
}

}  // namespace msgpack_light
