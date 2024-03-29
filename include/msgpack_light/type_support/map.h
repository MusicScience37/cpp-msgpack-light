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
 * \brief Definition of classes to support serialization of std::map instances.
 */
#pragma once

#include <map>

#include "msgpack_light/type_support/details/general_map_container_traits.h"
#include "msgpack_light/type_support/fwd.h"

namespace msgpack_light::type_support {

/*!
 * \brief Class to serialize std::map instances.
 *
 * \tparam Key Type of elements.
 * \tparam Value Type of mapped values.
 * \tparam Compare Type of functions to compare elements.
 * \tparam Allocator Type of allocators.
 */
template <typename Key, typename Value, typename Compare, typename Allocator>
struct serialization_traits<std::map<Key, Value, Compare, Allocator>>
    : public details::general_map_container_traits<
          std::map<Key, Value, Compare, Allocator>> {};

/*!
 * \brief Class to serialize std::multimap instances.
 *
 * \tparam Key Type of elements.
 * \tparam Value Type of mapped values.
 * \tparam Compare Type of functions to compare elements.
 * \tparam Allocator Type of allocators.
 */
template <typename Key, typename Value, typename Compare, typename Allocator>
struct serialization_traits<std::multimap<Key, Value, Compare, Allocator>>
    : public details::general_map_container_traits<
          std::multimap<Key, Value, Compare, Allocator>> {};

}  // namespace msgpack_light::type_support
