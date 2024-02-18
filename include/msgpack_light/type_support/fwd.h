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
 * \brief Forward declaration of classes to support serialization of data types.
 */
#pragma once

namespace msgpack_light::type_support {

/*!
 * \brief Class to define functions to serialize objects of various types.
 *
 * \tparam T Type of the object to serialize.
 * \tparam SFINAE Template parameter to use for SFINAE.
 *
 * Specializations of this template must implement a function with the following
 * signature.
 *
 * ```cpp
 * static void serialize(serialization_buffer& buffer, const T& value);
 * ```
 *
 * - The first argument is the buffer to serialize to.
 *   For the usage of the buffer, see msgpack_light::serialization_buffer class.
 * - The second argument is the serialized value.
 *   (Either `const T&` or `T` can be usable.)
 */
template <typename T, typename SFINAE = void>
struct serialization_traits;  // IWYU pragma: keep

}  // namespace msgpack_light::type_support
