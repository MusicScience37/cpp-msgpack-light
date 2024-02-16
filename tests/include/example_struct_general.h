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
 * \brief Definition of more general structs for tests.
 */
#pragma once

#include <cstdint>
#include <string>

#include "msgpack_light/type_support/struct.h"

namespace msgpack_light_test {

struct map_example_struct {
    std::int64_t id;
    std::string name;
    bool flag;
};

struct array_example_struct {
    std::string name;
    map_example_struct nested;
};

}  // namespace msgpack_light_test

MSGPACK_LIGHT_STRUCT_MAP(
    msgpack_light_test::map_example_struct, id, name, flag);
MSGPACK_LIGHT_STRUCT_ARRAY(
    msgpack_light_test::array_example_struct, name, nested);
