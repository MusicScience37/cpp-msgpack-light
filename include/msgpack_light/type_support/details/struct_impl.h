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
 * \brief Definition of macros to support serialization of struct types.
 */
#pragma once

#include "msgpack_light/serialization_buffer.h"
#include "msgpack_light/type_support/common.h"
#include "msgpack_light/type_support/fwd.h"

/*!
 * \brief Macro to generate class to support serialization of struct with 1
 * parameter.
 *
 * \param STRUCT Type of the struct.
 * \param PARAM1 Parameter.
 */
#define INTERNAL_MSGPACK_LIGHT_STRUCT_MAP1(STRUCT, PARAM1)                   \
    template <>                                                              \
    struct ::msgpack_light::type_support::serialization_traits<STRUCT> {     \
    public:                                                                  \
        static void serialize(::msgpack_light::serialization_buffer& buffer, \
            const STRUCT& value) {                                           \
            buffer.serialize_map_size(1);                                    \
            buffer.serialize(::std::string_view{#PARAM1});                   \
            buffer.serialize(value.PARAM1);                                  \
        }                                                                    \
    };

/*!
 * \brief Macro to generate class to support serialization of struct with 2
 * parameter.
 *
 * \param STRUCT Type of the struct.
 * \param PARAM1 Parameter.
 * \param PARAM2 Parameter.
 */
#define INTERNAL_MSGPACK_LIGHT_STRUCT_MAP2(STRUCT, PARAM1, PARAM2)           \
    template <>                                                              \
    struct ::msgpack_light::type_support::serialization_traits<STRUCT> {     \
    public:                                                                  \
        static void serialize(::msgpack_light::serialization_buffer& buffer, \
            const STRUCT& value) {                                           \
            buffer.serialize_map_size(2);                                    \
            buffer.serialize(::std::string_view{#PARAM1});                   \
            buffer.serialize(value.PARAM1);                                  \
            buffer.serialize(::std::string_view{#PARAM2});                   \
            buffer.serialize(value.PARAM2);                                  \
        }                                                                    \
    };

/*!
 * \brief Macro to generate class to support serialization of struct with 3
 * parameter.
 *
 * \param STRUCT Type of the struct.
 * \param PARAM1 Parameter.
 * \param PARAM2 Parameter.
 * \param PARAM3 Parameter.
 */
#define INTERNAL_MSGPACK_LIGHT_STRUCT_MAP3(STRUCT, PARAM1, PARAM2, PARAM3)   \
    template <>                                                              \
    struct ::msgpack_light::type_support::serialization_traits<STRUCT> {     \
    public:                                                                  \
        static void serialize(::msgpack_light::serialization_buffer& buffer, \
            const STRUCT& value) {                                           \
            buffer.serialize_map_size(3);                                    \
            buffer.serialize(::std::string_view{#PARAM1});                   \
            buffer.serialize(value.PARAM1);                                  \
            buffer.serialize(::std::string_view{#PARAM2});                   \
            buffer.serialize(value.PARAM2);                                  \
            buffer.serialize(::std::string_view{#PARAM3});                   \
            buffer.serialize(value.PARAM3);                                  \
        }                                                                    \
    };

/*!
 * \brief Macro to generate class to support serialization of struct with 4
 * parameter.
 *
 * \param STRUCT Type of the struct.
 * \param PARAM1 Parameter.
 * \param PARAM2 Parameter.
 * \param PARAM3 Parameter.
 * \param PARAM4 Parameter.
 */
#define INTERNAL_MSGPACK_LIGHT_STRUCT_MAP4(                                  \
    STRUCT, PARAM1, PARAM2, PARAM3, PARAM4)                                  \
    template <>                                                              \
    struct ::msgpack_light::type_support::serialization_traits<STRUCT> {     \
    public:                                                                  \
        static void serialize(::msgpack_light::serialization_buffer& buffer, \
            const STRUCT& value) {                                           \
            buffer.serialize_map_size(4);                                    \
            buffer.serialize(::std::string_view{#PARAM1});                   \
            buffer.serialize(value.PARAM1);                                  \
            buffer.serialize(::std::string_view{#PARAM2});                   \
            buffer.serialize(value.PARAM2);                                  \
            buffer.serialize(::std::string_view{#PARAM3});                   \
            buffer.serialize(value.PARAM3);                                  \
            buffer.serialize(::std::string_view{#PARAM4});                   \
            buffer.serialize(value.PARAM4);                                  \
        }                                                                    \
    };

/*!
 * \brief Macro to generate class to support serialization of struct with 5
 * parameter.
 *
 * \param STRUCT Type of the struct.
 * \param PARAM1 Parameter.
 * \param PARAM2 Parameter.
 * \param PARAM3 Parameter.
 * \param PARAM4 Parameter.
 * \param PARAM5 Parameter.
 */
#define INTERNAL_MSGPACK_LIGHT_STRUCT_MAP5(                                  \
    STRUCT, PARAM1, PARAM2, PARAM3, PARAM4, PARAM5)                          \
    template <>                                                              \
    struct ::msgpack_light::type_support::serialization_traits<STRUCT> {     \
    public:                                                                  \
        static void serialize(::msgpack_light::serialization_buffer& buffer, \
            const STRUCT& value) {                                           \
            buffer.serialize_map_size(5);                                    \
            buffer.serialize(::std::string_view{#PARAM1});                   \
            buffer.serialize(value.PARAM1);                                  \
            buffer.serialize(::std::string_view{#PARAM2});                   \
            buffer.serialize(value.PARAM2);                                  \
            buffer.serialize(::std::string_view{#PARAM3});                   \
            buffer.serialize(value.PARAM3);                                  \
            buffer.serialize(::std::string_view{#PARAM4});                   \
            buffer.serialize(value.PARAM4);                                  \
            buffer.serialize(::std::string_view{#PARAM5});                   \
            buffer.serialize(value.PARAM5);                                  \
        }                                                                    \
    };

/*!
 * \brief Macro to generate class to support serialization of struct with 6
 * parameter.
 *
 * \param STRUCT Type of the struct.
 * \param PARAM1 Parameter.
 * \param PARAM2 Parameter.
 * \param PARAM3 Parameter.
 * \param PARAM4 Parameter.
 * \param PARAM5 Parameter.
 * \param PARAM6 Parameter.
 */
#define INTERNAL_MSGPACK_LIGHT_STRUCT_MAP6(                                  \
    STRUCT, PARAM1, PARAM2, PARAM3, PARAM4, PARAM5, PARAM6)                  \
    template <>                                                              \
    struct ::msgpack_light::type_support::serialization_traits<STRUCT> {     \
    public:                                                                  \
        static void serialize(::msgpack_light::serialization_buffer& buffer, \
            const STRUCT& value) {                                           \
            buffer.serialize_map_size(6);                                    \
            buffer.serialize(::std::string_view{#PARAM1});                   \
            buffer.serialize(value.PARAM1);                                  \
            buffer.serialize(::std::string_view{#PARAM2});                   \
            buffer.serialize(value.PARAM2);                                  \
            buffer.serialize(::std::string_view{#PARAM3});                   \
            buffer.serialize(value.PARAM3);                                  \
            buffer.serialize(::std::string_view{#PARAM4});                   \
            buffer.serialize(value.PARAM4);                                  \
            buffer.serialize(::std::string_view{#PARAM5});                   \
            buffer.serialize(value.PARAM5);                                  \
            buffer.serialize(::std::string_view{#PARAM6});                   \
            buffer.serialize(value.PARAM6);                                  \
        }                                                                    \
    };

/*!
 * \brief Macro to generate class to support serialization of struct with 7
 * parameter.
 *
 * \param STRUCT Type of the struct.
 * \param PARAM1 Parameter.
 * \param PARAM2 Parameter.
 * \param PARAM3 Parameter.
 * \param PARAM4 Parameter.
 * \param PARAM5 Parameter.
 * \param PARAM6 Parameter.
 * \param PARAM7 Parameter.
 */
#define INTERNAL_MSGPACK_LIGHT_STRUCT_MAP7(                                  \
    STRUCT, PARAM1, PARAM2, PARAM3, PARAM4, PARAM5, PARAM6, PARAM7)          \
    template <>                                                              \
    struct ::msgpack_light::type_support::serialization_traits<STRUCT> {     \
    public:                                                                  \
        static void serialize(::msgpack_light::serialization_buffer& buffer, \
            const STRUCT& value) {                                           \
            buffer.serialize_map_size(7);                                    \
            buffer.serialize(::std::string_view{#PARAM1});                   \
            buffer.serialize(value.PARAM1);                                  \
            buffer.serialize(::std::string_view{#PARAM2});                   \
            buffer.serialize(value.PARAM2);                                  \
            buffer.serialize(::std::string_view{#PARAM3});                   \
            buffer.serialize(value.PARAM3);                                  \
            buffer.serialize(::std::string_view{#PARAM4});                   \
            buffer.serialize(value.PARAM4);                                  \
            buffer.serialize(::std::string_view{#PARAM5});                   \
            buffer.serialize(value.PARAM5);                                  \
            buffer.serialize(::std::string_view{#PARAM6});                   \
            buffer.serialize(value.PARAM6);                                  \
            buffer.serialize(::std::string_view{#PARAM7});                   \
            buffer.serialize(value.PARAM7);                                  \
        }                                                                    \
    };

/*!
 * \brief Macro to generate class to support serialization of struct with 8
 * parameter.
 *
 * \param STRUCT Type of the struct.
 * \param PARAM1 Parameter.
 * \param PARAM2 Parameter.
 * \param PARAM3 Parameter.
 * \param PARAM4 Parameter.
 * \param PARAM5 Parameter.
 * \param PARAM6 Parameter.
 * \param PARAM7 Parameter.
 * \param PARAM8 Parameter.
 */
#define INTERNAL_MSGPACK_LIGHT_STRUCT_MAP8(                                  \
    STRUCT, PARAM1, PARAM2, PARAM3, PARAM4, PARAM5, PARAM6, PARAM7, PARAM8)  \
    template <>                                                              \
    struct ::msgpack_light::type_support::serialization_traits<STRUCT> {     \
    public:                                                                  \
        static void serialize(::msgpack_light::serialization_buffer& buffer, \
            const STRUCT& value) {                                           \
            buffer.serialize_map_size(8);                                    \
            buffer.serialize(::std::string_view{#PARAM1});                   \
            buffer.serialize(value.PARAM1);                                  \
            buffer.serialize(::std::string_view{#PARAM2});                   \
            buffer.serialize(value.PARAM2);                                  \
            buffer.serialize(::std::string_view{#PARAM3});                   \
            buffer.serialize(value.PARAM3);                                  \
            buffer.serialize(::std::string_view{#PARAM4});                   \
            buffer.serialize(value.PARAM4);                                  \
            buffer.serialize(::std::string_view{#PARAM5});                   \
            buffer.serialize(value.PARAM5);                                  \
            buffer.serialize(::std::string_view{#PARAM6});                   \
            buffer.serialize(value.PARAM6);                                  \
            buffer.serialize(::std::string_view{#PARAM7});                   \
            buffer.serialize(value.PARAM7);                                  \
            buffer.serialize(::std::string_view{#PARAM8});                   \
            buffer.serialize(value.PARAM8);                                  \
        }                                                                    \
    };

/*!
 * \brief Macro to generate class to support serialization of struct with 9
 * parameter.
 *
 * \param STRUCT Type of the struct.
 * \param PARAM1 Parameter.
 * \param PARAM2 Parameter.
 * \param PARAM3 Parameter.
 * \param PARAM4 Parameter.
 * \param PARAM5 Parameter.
 * \param PARAM6 Parameter.
 * \param PARAM7 Parameter.
 * \param PARAM8 Parameter.
 * \param PARAM9 Parameter.
 */
#define INTERNAL_MSGPACK_LIGHT_STRUCT_MAP9(STRUCT, PARAM1, PARAM2, PARAM3,   \
    PARAM4, PARAM5, PARAM6, PARAM7, PARAM8, PARAM9)                          \
    template <>                                                              \
    struct ::msgpack_light::type_support::serialization_traits<STRUCT> {     \
    public:                                                                  \
        static void serialize(::msgpack_light::serialization_buffer& buffer, \
            const STRUCT& value) {                                           \
            buffer.serialize_map_size(9);                                    \
            buffer.serialize(::std::string_view{#PARAM1});                   \
            buffer.serialize(value.PARAM1);                                  \
            buffer.serialize(::std::string_view{#PARAM2});                   \
            buffer.serialize(value.PARAM2);                                  \
            buffer.serialize(::std::string_view{#PARAM3});                   \
            buffer.serialize(value.PARAM3);                                  \
            buffer.serialize(::std::string_view{#PARAM4});                   \
            buffer.serialize(value.PARAM4);                                  \
            buffer.serialize(::std::string_view{#PARAM5});                   \
            buffer.serialize(value.PARAM5);                                  \
            buffer.serialize(::std::string_view{#PARAM6});                   \
            buffer.serialize(value.PARAM6);                                  \
            buffer.serialize(::std::string_view{#PARAM7});                   \
            buffer.serialize(value.PARAM7);                                  \
            buffer.serialize(::std::string_view{#PARAM8});                   \
            buffer.serialize(value.PARAM8);                                  \
            buffer.serialize(::std::string_view{#PARAM9});                   \
            buffer.serialize(value.PARAM9);                                  \
        }                                                                    \
    };

/*!
 * \brief Macro to generate class to support serialization of struct with 10
 * parameter.
 *
 * \param STRUCT Type of the struct.
 * \param PARAM1 Parameter.
 * \param PARAM2 Parameter.
 * \param PARAM3 Parameter.
 * \param PARAM4 Parameter.
 * \param PARAM5 Parameter.
 * \param PARAM6 Parameter.
 * \param PARAM7 Parameter.
 * \param PARAM8 Parameter.
 * \param PARAM9 Parameter.
 * \param PARAM10 Parameter.
 */
#define INTERNAL_MSGPACK_LIGHT_STRUCT_MAP10(STRUCT, PARAM1, PARAM2, PARAM3,  \
    PARAM4, PARAM5, PARAM6, PARAM7, PARAM8, PARAM9, PARAM10)                 \
    template <>                                                              \
    struct ::msgpack_light::type_support::serialization_traits<STRUCT> {     \
    public:                                                                  \
        static void serialize(::msgpack_light::serialization_buffer& buffer, \
            const STRUCT& value) {                                           \
            buffer.serialize_map_size(10);                                   \
            buffer.serialize(::std::string_view{#PARAM1});                   \
            buffer.serialize(value.PARAM1);                                  \
            buffer.serialize(::std::string_view{#PARAM2});                   \
            buffer.serialize(value.PARAM2);                                  \
            buffer.serialize(::std::string_view{#PARAM3});                   \
            buffer.serialize(value.PARAM3);                                  \
            buffer.serialize(::std::string_view{#PARAM4});                   \
            buffer.serialize(value.PARAM4);                                  \
            buffer.serialize(::std::string_view{#PARAM5});                   \
            buffer.serialize(value.PARAM5);                                  \
            buffer.serialize(::std::string_view{#PARAM6});                   \
            buffer.serialize(value.PARAM6);                                  \
            buffer.serialize(::std::string_view{#PARAM7});                   \
            buffer.serialize(value.PARAM7);                                  \
            buffer.serialize(::std::string_view{#PARAM8});                   \
            buffer.serialize(value.PARAM8);                                  \
            buffer.serialize(::std::string_view{#PARAM9});                   \
            buffer.serialize(value.PARAM9);                                  \
            buffer.serialize(::std::string_view{#PARAM10});                  \
            buffer.serialize(value.PARAM10);                                 \
        }                                                                    \
    };
