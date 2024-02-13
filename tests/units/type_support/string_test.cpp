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
 * \brief Test of classes to support serialization of strings.
 */
#include "msgpack_light/type_support/string.h"

#include <tuple>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "msgpack_light/binary.h"
#include "msgpack_light/memory_output_stream.h"
#include "msgpack_light/serialization_buffer.h"

TEST_CASE("msgpack_light::type_support::serialization_traits<std::string>") {
    using msgpack_light::binary;
    using msgpack_light::memory_output_stream;
    using msgpack_light::serialization_buffer;

    SECTION("serialize") {
        std::string value{};
        binary expected_binary{};
        std::tie(value, expected_binary) = GENERATE(table<std::string,
            binary>({{std::string(), binary("A0")},
            {std::string("abc"), binary("A3616263")},
            {std::string(
                 "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"),
                binary(
                    "D934"
                    "6162636465666768696A6B6C6D6E6F707172737475767778797A"
                    "4142434445464748494A4B4C4D4E4F505152535455565758595A")}}));

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }
}

TEST_CASE(
    "msgpack_light::type_support::serialization_traits<std::string_view>") {
    using msgpack_light::binary;
    using msgpack_light::memory_output_stream;
    using msgpack_light::serialization_buffer;

    SECTION("serialize") {
        std::string_view value{};
        binary expected_binary{};
        std::tie(value, expected_binary) = GENERATE(table<std::string_view,
            binary>({{std::string_view(), binary("A0")},
            {std::string_view("abc"), binary("A3616263")},
            {std::string_view(
                 "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"),
                binary(
                    "D934"
                    "6162636465666768696A6B6C6D6E6F707172737475767778797A"
                    "4142434445464748494A4B4C4D4E4F505152535455565758595A")}}));

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }
}
