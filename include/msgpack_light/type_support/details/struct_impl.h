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
 * \param PARAM1 First parameter.
 */
#define INTERNAL_MSGPACK_LIGHT_STRUCT_MAP2(STRUCT, PARAM1)                   \
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
