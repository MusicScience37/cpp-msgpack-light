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
 * \brief Test of classes to support serialization of std::tuple objects.
 */
#include "msgpack_light/type_support/tuple.h"

#include <catch2/catch_test_macros.hpp>

#include "msgpack_light/binary.h"
#include "msgpack_light/memory_output_stream.h"
#include "msgpack_light/serialization_buffer.h"
#include "msgpack_light/type_support/common.h"

TEST_CASE(
    "msgpack_light::type_support::serialization_traits<std::tuple<...>>") {
    using msgpack_light::binary;
    using msgpack_light::memory_output_stream;
    using msgpack_light::serialization_buffer;

    SECTION("serialize an empty tuple") {
        const std::tuple<> value;
        const auto expected_binary = binary("90");

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize std::tuple<int>") {
        const auto value = std::make_tuple(123);
        const auto expected_binary = binary("917B");

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize std::tuple<std::string, bool>") {
        const auto value = std::make_tuple(std::string("ABC"), false);
        const auto expected_binary = binary(
            "92"        // size
            "A3414243"  // "ABC"
            "C2"        // false
        );

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize std::tuple<std::vector<int>, std::nullptr_t, int>") {
        const auto value = std::make_tuple(std::vector{1, 2}, nullptr, 12345);
        const auto expected_binary = binary(
            "93"      // size
            "920102"  // std::vector{1, 2}
            "C0"      // nullptr
            "CD3039"  // 12345
        );

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }
}
