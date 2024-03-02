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
 * \brief Definition of helper functions for implementation of object class.
 */
#pragma once

#include <cstring>

#include "msgpack_light/details/allocator_wrapper.h"
#include "msgpack_light/details/object_data.h"
#include "msgpack_light/object_data_type.h"

namespace msgpack_light::details {

/*!
 * \brief Clear data.
 *
 * \tparam Allocator Type of the allocator.
 * \param[in,out] data Data.
 * \param[in] allocator Allocator.
 */
template <typename Allocator>
inline void clear_object_data(
    object_data& data, allocator_wrapper<Allocator>& allocator) noexcept {
    switch (data.type) {
    case object_data_type::string:
        allocator.deallocate_char(data.data.string_value.data);
        break;
    case object_data_type::binary:
        allocator.deallocate_unsigned_char(data.data.binary_value.data);
        break;
    case object_data_type::array:
        for (std::size_t i = 0; i < data.data.array_value.size; ++i) {
            clear_object_data(data.data.array_value.data[i], allocator);
        }
        allocator.deallocate_object_data(data.data.array_value.data);
        break;
    case object_data_type::map:
        for (std::size_t i = 0; i < data.data.map_value.size; ++i) {
            clear_object_data(data.data.map_value.data[i].key, allocator);
            clear_object_data(data.data.map_value.data[i].value, allocator);
        }
        allocator.deallocate_key_value_pair_data(data.data.map_value.data);
        break;
    case object_data_type::extension:
        allocator.deallocate_unsigned_char(data.data.extension_value.data);
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
inline void copy_object_data(object_data& to, const object_data& from,
    allocator_wrapper<Allocator>& allocator) {
    switch (from.type) {
    case object_data_type::string:
        to.data.string_value.data =
            allocator.allocate_char(from.data.string_value.size);
        std::memcpy(to.data.string_value.data, from.data.string_value.data,
            from.data.string_value.size);
        to.data.string_value.size = from.data.string_value.size;
        to.type = object_data_type::string;
        break;
    case object_data_type::binary:
        to.data.binary_value.data =
            allocator.allocate_unsigned_char(from.data.binary_value.size);
        std::memcpy(to.data.binary_value.data, from.data.binary_value.data,
            from.data.binary_value.size);
        to.data.binary_value.size = from.data.binary_value.size;
        to.type = object_data_type::binary;
        break;
    case object_data_type::array:
        to.data.array_value.data =
            allocator.allocate_object_data(from.data.array_value.size);
        to.data.array_value.size = from.data.array_value.size;
        to.type = object_data_type::array;
        for (std::size_t i = 0; i < from.data.array_value.size; ++i) {
            copy_object_data(to.data.array_value.data[i],
                from.data.array_value.data[i], allocator);
        }
        break;
    case object_data_type::map:
        to.data.map_value.data =
            allocator.allocate_key_value_pair_data(from.data.map_value.size);
        to.data.map_value.size = from.data.map_value.size;
        to.type = object_data_type::map;
        for (std::size_t i = 0; i < from.data.map_value.size; ++i) {
            copy_object_data(to.data.map_value.data[i].key,
                from.data.map_value.data[i].key, allocator);
            copy_object_data(to.data.map_value.data[i].value,
                from.data.map_value.data[i].value, allocator);
        }
        break;
    case object_data_type::extension:
        to.data.extension_value.data =
            allocator.allocate_unsigned_char(from.data.extension_value.size);
        std::memcpy(to.data.extension_value.data,
            from.data.extension_value.data, from.data.extension_value.size);
        to.data.extension_value.size = from.data.extension_value.size;
        to.data.extension_value.type = from.data.extension_value.type;
        to.type = object_data_type::extension;
        break;
    default:
        to = from;
        break;
    };
}

}  // namespace msgpack_light::details
