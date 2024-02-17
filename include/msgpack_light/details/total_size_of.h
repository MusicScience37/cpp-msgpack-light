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
 * \brief Definition of total_size_of variable.
 */
#pragma once

#include <cstddef>

namespace msgpack_light::details {

/*!
 * \brief Get the total size of types in template arguments.
 *
 * \tparam T Types.
 */
template <typename... T>
struct total_size_of_impl;

/*!
 * \brief Get the total size of types in template arguments.
 *
 * \tparam First First type.
 * \tparam Remaining Remaining types.
 */
template <typename First, typename... Remaining>
struct total_size_of_impl<First, Remaining...> {
    //! Total size.
    static constexpr std::size_t value =
        sizeof(First) + total_size_of_impl<Remaining...>::value;
};

/*!
 * \brief Get the total size of types in template arguments.
 *
 * \tparam T Type.
 */
template <typename T>
struct total_size_of_impl<T> {
    //! Total size.
    static constexpr std::size_t value = sizeof(T);
};

/*!
 * \brief Get the total size of types in template arguments.
 *
 * \tparam T Types.
 */
template <typename... T>
constexpr std::size_t total_size_of = total_size_of_impl<T...>::value;

}  // namespace msgpack_light::details
