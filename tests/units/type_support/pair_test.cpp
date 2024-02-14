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
 * \brief Test of classes to support serialization of std::pair objects.
 */
#include "msgpack_light/type_support/pair.h"

#include <string>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "msgpack_light/binary.h"
#include "msgpack_light/memory_output_stream.h"
#include "msgpack_light/serialization_buffer.h"
#include "msgpack_light/type_support/common.h"  // IWYU pragma: keep

TEST_CASE("msgpack_light::type_support::serialization_traits<std::pair<...>>") {
    using msgpack_light::binary;
    using msgpack_light::memory_output_stream;
    using msgpack_light::serialization_buffer;

    SECTION("serialize std::pair<bool, int>") {
        const auto value = std::make_pair(true, 12345);
        const auto expected_binary = binary("92C3CD3039");

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize std::pair<std::string, std::vector<int>>") {
        const auto value =
            std::make_pair(std::string("abc"), std::vector{1, 2, 3});
        const auto expected_binary = binary(
            "92"        // size
            "A3616263"  // "abc"
            "93010203"  // std::vector{1, 2, 3}
        );

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }
}
