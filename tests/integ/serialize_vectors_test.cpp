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
 * \brief Test to serialize vectors.
 */
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_range.hpp>
#include <msgpack.hpp>

#include "msgpack_light/binary.h"
#include "msgpack_light/serialize.h"
#include "msgpack_light/type_support/vector.h"

TEST_CASE("serialize vectors of unsigned char") {
    using msgpack_light::binary;
    using msgpack_light::serialize;

    SECTION("serialize sample binaries") {
        const std::vector<unsigned char> value =
            GENERATE(std::vector<unsigned char>(),
                std::vector<unsigned char>{static_cast<unsigned char>(0xAB)},
                std::vector<unsigned char>{static_cast<unsigned char>(0xAB),
                    static_cast<unsigned char>(0xCD)});

        const binary serialized = serialize(value);

        const msgpack::object_handle deserialized = msgpack::unpack(
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            reinterpret_cast<const char*>(serialized.data()),
            serialized.size());

        CHECK(deserialized->as<std::vector<unsigned char>>() == value);
    }

    SECTION("serialize binaries with variable sizes") {
        const std::size_t size = GENERATE(
            range(static_cast<std::size_t>(0), static_cast<std::size_t>(1000)));
        INFO("size = " << size);

        const auto value =
            std::vector<unsigned char>(size, static_cast<unsigned char>(0xAA));

        const binary serialized = serialize(value);

        const msgpack::object_handle deserialized = msgpack::unpack(
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            reinterpret_cast<const char*>(serialized.data()),
            serialized.size());

        CHECK(deserialized->as<std::vector<unsigned char>>() == value);
    }
}
