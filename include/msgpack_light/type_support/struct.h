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

#include "msgpack_light/details/concatenate_macro.h"
#include "msgpack_light/details/count_arguments_macro.h"
#include "msgpack_light/details/invoke_macro.h"
#include "msgpack_light/type_support/details/struct_impl.h"

/*!
 * \brief Macro to generate class to support serialization of struct into maps.
 *
 * \param STRUCT Type of the struct. Remaining arguments are treated as
 * parameter names.
 */
#define MSGPACK_LIGHT_STRUCT_MAP(STRUCT, ...)                                 \
    INTERNAL_MSGPACK_LIGHT_INVOKE_MACRO(                                      \
        INTERNAL_MSGPACK_LIGHT_CONCATENATE(INTERNAL_MSGPACK_LIGHT_STRUCT_MAP, \
            INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS(__VA_ARGS__)),             \
        (STRUCT, __VA_ARGS__))
