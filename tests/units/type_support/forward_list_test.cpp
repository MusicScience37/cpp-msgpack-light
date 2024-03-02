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
 * \brief Test of classes to support serialization of std::forward_list
 * instances.
 */
#include "msgpack_light/type_support/forward_list.h"

#include <tuple>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "msgpack_light/binary.h"
#include "msgpack_light/memory_output_stream.h"
#include "msgpack_light/serialization_buffer.h"
#include "msgpack_light/type_support/common.h"  // IWYU pragma: keep

TEST_CASE(
    "msgpack_light::type_support::serialization_traits<std::forward_list<int>"
    ">") {
    using msgpack_light::binary;
    using msgpack_light::memory_output_stream;
    using msgpack_light::serialization_buffer;

    SECTION("serialize") {
        std::forward_list<int> value;
        binary expected_binary;
        std::tie(value, expected_binary) =
            GENERATE(table<std::forward_list<int>, binary>(
                {{std::forward_list<int>(), binary("90")},
                    {std::forward_list<int>{0x2A}, binary("912A")},
                    {std::forward_list<int>{0x2A, 0x3B}, binary("922A3B")},
                    {std::forward_list<int>{0x2A, 0x3B, 0x4C},
                        binary("932A3B4C")}}));

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }
}
