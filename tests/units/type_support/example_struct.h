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
 * \brief Definition of structs for tests.
 */
#pragma once

// TODO change header to a non-internal one.
#include "msgpack_light/type_support/details/struct_impl.h"

namespace msgpack_light_test {

/*!
 * \brief Example of struct with 1 parameter serialized into maps.
 */
struct map_example_struct1 {
    int param1;
};

}  // namespace msgpack_light_test

INTERNAL_MSGPACK_LIGHT_STRUCT_MAP2(
    msgpack_light_test::map_example_struct1, param1);
