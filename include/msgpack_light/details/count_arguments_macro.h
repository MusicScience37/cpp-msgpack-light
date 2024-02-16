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
 * \brief Definition of INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS macro.
 */
#pragma once

/*!
 * \brief Internal macro to implement INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS
 * macro.
 */
#define INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS_IMPL_GET_COUNT( \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...)           \
    N

/*!
 * \brief Internal macro to implement INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS
 * macro.
 */
#define INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS_IMPL_NUMBERS() \
    10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

/*!
 * \brief Internal macro to implement INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS
 * macro.
 */
#define INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS_IMPL_INVOKE(macro, args) \
    macro args

/*!
 * \brief Internal macro to implement INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS
 * macro.
 */
#define INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS_IMPL(...) \
    INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS_IMPL_INVOKE(  \
        INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS_IMPL_GET_COUNT, (__VA_ARGS__))

/*!
 * \brief Macro to count number of arguments.
 *
 * \warning This macro works only numbers from 1 to 5.
 */
#define INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS(...) \
    INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS_IMPL(    \
        __VA_ARGS__, INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS_IMPL_NUMBERS())
