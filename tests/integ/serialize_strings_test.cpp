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
 * \brief Test to serialize strings.
 */
#include <cstdlib>
#include <string>

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_range.hpp>
#include <msgpack.hpp>

#include "msgpack_light/binary.h"
#include "msgpack_light/serialize.h"

TEST_CASE("serialize strings") {
    using msgpack_light::binary;
    using msgpack_light::serialize;

    SECTION("serialize sample strings") {
        const std::string value =
            GENERATE(std::string(), std::string("abc"), std::string("abcde"),
                std::string(
                    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"));
        INFO("value = " << value);

        const binary serialized = serialize(value);

        const msgpack::object_handle deserialized =
            msgpack::unpack(reinterpret_cast<const char*>(serialized.data()),
                serialized.size());

        CHECK(deserialized->as<std::string>() == value);
    }

    SECTION("serialize strings with variable sizes") {
        const std::size_t size = GENERATE(
            range(static_cast<std::size_t>(0), static_cast<std::size_t>(1000)));
        INFO("size = " << size);

        const auto value = std::string(size, 'A');

        const binary serialized = serialize(value);

        const msgpack::object_handle deserialized =
            msgpack::unpack(reinterpret_cast<const char*>(serialized.data()),
                serialized.size());

        CHECK(deserialized->as<std::string>() == value);
    }
}
