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
 * \brief Test to serialize struct.
 */
#include <catch2/catch_test_macros.hpp>

#include "example_struct.h"
#include "msgpack_light/binary.h"
#include "msgpack_light/serialize.h"

TEST_CASE("MSGPACK_LIGHT_STRUCT_MAP") {
    using msgpack_light::binary;
    using msgpack_light::serialize;
    using msgpack_light_test::map_example_struct1;

    SECTION("serialize a struct with 1 parameter into a map") {
        const auto value = map_example_struct1{123};
        const auto expected_binary = binary(
            "81"              // fixmap with 1 pair
            "A6706172616D31"  // "param1"
            "7B"              // 123
        );

        CHECK(serialize(value) == expected_binary);
    }
}
