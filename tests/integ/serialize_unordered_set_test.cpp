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
 * \brief Test to serialize std::unordered_set objects.
 */
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_range.hpp>
#include <msgpack.hpp>

#include "msgpack_light/binary.h"
#include "msgpack_light/serialize.h"
#include "msgpack_light/type_support/unordered_set.h"

TEST_CASE("serialize std::unordered_set objects of int") {
    using msgpack_light::binary;
    using msgpack_light::serialize;

    SECTION("serialize sample std::unordered_set objects") {
        const std::unordered_set<int> value =
            GENERATE(std::unordered_set<int>(), std::unordered_set<int>{0x2A},
                std::unordered_set<int>{0x2A, 0x3B},
                std::unordered_set<int>{0x2A, 0x3B, 0x4C});

        const binary serialized = serialize(value);

        const msgpack::object_handle deserialized = msgpack::unpack(
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            reinterpret_cast<const char*>(serialized.data()),
            serialized.size());

        CHECK(deserialized->as<std::unordered_set<int>>() == value);
    }
}

TEST_CASE("serialize std::unordered_multiset objects of int") {
    using msgpack_light::binary;
    using msgpack_light::serialize;

    SECTION("serialize sample std::unordered_multiset objects") {
        const std::unordered_multiset<int> value = GENERATE(
            std::unordered_multiset<int>(), std::unordered_multiset<int>{0x2A},
            std::unordered_multiset<int>{0x2A, 0x3B},
            std::unordered_multiset<int>{0x2A, 0x3B, 0x4C});

        const binary serialized = serialize(value);

        const msgpack::object_handle deserialized = msgpack::unpack(
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            reinterpret_cast<const char*>(serialized.data()),
            serialized.size());

        CHECK(deserialized->as<std::unordered_multiset<int>>() == value);
    }
}
