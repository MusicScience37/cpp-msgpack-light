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
 * \brief Definition of classes to support serialization of std::unordered_set
 * instances.
 */
#pragma once

#include <unordered_set>

#include "msgpack_light/type_support/details/general_array_container_traits.h"
#include "msgpack_light/type_support/fwd.h"

namespace msgpack_light::type_support {

/*!
 * \brief Class to serialize std::unordered_set instances.
 *
 * \tparam Key Type of elements.
 * \tparam Hash Type of the hash function.
 * \tparam KeyEqual Type of the function to compare keys.
 * \tparam Allocator Type of allocators.
 */
template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
struct serialization_traits<std::unordered_set<Key, Hash, KeyEqual, Allocator>>
    : public details::general_array_container_traits<
          std::unordered_set<Key, Hash, KeyEqual, Allocator>> {};

/*!
 * \brief Class to serialize std::unordered_multiset instances.
 *
 * \tparam Key Type of elements.
 * \tparam Hash Type of the hash function.
 * \tparam KeyEqual Type of the function to compare keys.
 * \tparam Allocator Type of allocators.
 */
template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
struct serialization_traits<
    std::unordered_multiset<Key, Hash, KeyEqual, Allocator>>
    : public details::general_array_container_traits<
          std::unordered_multiset<Key, Hash, KeyEqual, Allocator>> {};

}  // namespace msgpack_light::type_support
