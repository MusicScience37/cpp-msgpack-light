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
 * \brief Test of classes to support serialization of std::map instances.
 */
#include "msgpack_light/type_support/map.h"

#include <string>
#include <tuple>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "msgpack_light/binary.h"
#include "msgpack_light/memory_output_stream.h"
#include "msgpack_light/serialization_buffer.h"
#include "msgpack_light/type_support/common.h"  // IWYU pragma: keep

TEST_CASE(
    "msgpack_light::type_support::serialization_traits<std::map<int, "
    "std::string>>") {
    using msgpack_light::binary;
    using msgpack_light::memory_output_stream;
    using msgpack_light::serialization_buffer;

    SECTION("serialize") {
        std::map<int, std::string> value;
        binary expected_binary;
        std::tie(value, expected_binary) =
            GENERATE(table<std::map<int, std::string>, binary>(
                {{std::map<int, std::string>{}, binary("80")},
                    {std::map<int, std::string>{{1, "a"}},
                        binary("81"
                               "01A161")},
                    {std::map<int, std::string>{{1, "a"}, {2, "b"}},
                        binary("82"
                               "01A161"
                               "02A162")},
                    {std::map<int, std::string>{{1, "a"}, {2, "b"}, {3, "c"}},
                        binary("83"
                               "01A161"
                               "02A162"
                               "03A163")}}));

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }
}

TEST_CASE(
    "msgpack_light::type_support::serialization_traits<std::multimap<int, "
    "std::string>>") {
    using msgpack_light::binary;
    using msgpack_light::memory_output_stream;
    using msgpack_light::serialization_buffer;

    SECTION("serialize") {
        std::multimap<int, std::string> value;
        binary expected_binary;
        std::tie(value, expected_binary) =
            GENERATE(table<std::multimap<int, std::string>, binary>(
                {{std::multimap<int, std::string>{}, binary("80")},
                    {std::multimap<int, std::string>{{1, "a"}},
                        binary("81"
                               "01A161")},
                    {std::multimap<int, std::string>{{1, "a"}, {2, "b"}},
                        binary("82"
                               "01A161"
                               "02A162")},
                    {std::multimap<int, std::string>{
                         {1, "a"}, {2, "b"}, {3, "c"}},
                        binary("83"
                               "01A161"
                               "02A162"
                               "03A163")}}));

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }
}
