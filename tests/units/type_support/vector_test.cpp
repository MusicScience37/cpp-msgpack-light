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
 * \brief Test of classes to support serialization of vectors.
 */
#include "msgpack_light/type_support/vector.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "msgpack_light/binary.h"
#include "msgpack_light/memory_output_stream.h"
#include "msgpack_light/serialization_buffer.h"
#include "msgpack_light/type_support/common.h"

TEST_CASE(
    "msgpack_light::type_support::serialization_traits<std::vector<int>>") {
    using msgpack_light::binary;
    using msgpack_light::memory_output_stream;
    using msgpack_light::serialization_buffer;

    SECTION("serialize") {
        std::vector<int> value;
        binary expected_binary;
        std::tie(value, expected_binary) = GENERATE(
            table<std::vector<int>, binary>({{std::vector<int>(), binary("90")},
                {std::vector<int>{0x2A}, binary("912A")},
                {std::vector<int>{0x2A, 0x3B}, binary("922A3B")},
                {std::vector<int>{0x2A, 0x3B, 0x4C}, binary("932A3B4C")}}));

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }
}

TEST_CASE(
    "msgpack_light::type_support::serialization_traits<std::vector<unsigned "
    "char>>") {
    using msgpack_light::binary;
    using msgpack_light::memory_output_stream;
    using msgpack_light::serialization_buffer;

    SECTION("serialize") {
        std::vector<unsigned char> value;
        binary expected_binary;
        std::tie(value, expected_binary) =
            GENERATE(table<std::vector<unsigned char>,
                binary>({{std::vector<unsigned char>(), binary("C400")},
                {std::vector<unsigned char>{static_cast<unsigned char>(0xAB)},
                    binary("C401AB")},
                {std::vector<unsigned char>{static_cast<unsigned char>(0xAB),
                     static_cast<unsigned char>(0xCD)},
                    binary("C402ABCD")}}));

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }
}
