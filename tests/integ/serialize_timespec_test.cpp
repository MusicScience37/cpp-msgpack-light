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
 * \brief Test to serialize std::timespec objects.
 */
#include <ctime>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <msgpack.hpp>

#include "msgpack_light/binary.h"
#include "msgpack_light/memory_output_stream.h"
#include "msgpack_light/serialization_buffer.h"
#include "msgpack_light/serialize.h"
#include "msgpack_light/type_support/timespec.h"

TEST_CASE("msgpack_light::type_support::serialization_traits<std::timespec>") {
    using msgpack_light::binary;
    using msgpack_light::serialize;

    SECTION("serialize timestamp 32") {
        std::timespec value{};
        value.tv_sec = static_cast<std::time_t>(0xABCD1234U);  // NOLINT
        value.tv_nsec = 0;

        const binary serialized = serialize(value);

        const msgpack::object_handle deserialized = msgpack::unpack(
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            reinterpret_cast<const char*>(serialized.data()),
            serialized.size());

        std::timespec deserialized_value{};
        REQUIRE_NOTHROW(deserialized_value = deserialized->as<std::timespec>());
        CHECK(deserialized_value.tv_sec == value.tv_sec);
        CHECK(deserialized_value.tv_nsec == value.tv_nsec);
    }

    SECTION("serialize timestamp 64") {
        std::timespec value{};
        value.tv_sec = static_cast<std::time_t>(0x2ABCD1234U);  // NOLINT
        value.tv_nsec = 0x75BCD15;                              // NOLINT

        const binary serialized = serialize(value);

        const msgpack::object_handle deserialized = msgpack::unpack(
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            reinterpret_cast<const char*>(serialized.data()),
            serialized.size());

        std::timespec deserialized_value{};
        REQUIRE_NOTHROW(deserialized_value = deserialized->as<std::timespec>());
        CHECK(deserialized_value.tv_sec == value.tv_sec);
        CHECK(deserialized_value.tv_nsec == value.tv_nsec);
    }

    SECTION("serialize timestamp 96") {
        std::timespec value{};
        value.tv_sec = static_cast<std::time_t>(0x123456789ABCDEF1U);  // NOLINT
        value.tv_nsec = 0x75BCD15;                                     // NOLINT

        const binary serialized = serialize(value);

        const msgpack::object_handle deserialized = msgpack::unpack(
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            reinterpret_cast<const char*>(serialized.data()),
            serialized.size());

        std::timespec deserialized_value{};
        REQUIRE_NOTHROW(deserialized_value = deserialized->as<std::timespec>());
        CHECK(deserialized_value.tv_sec == value.tv_sec);
        CHECK(deserialized_value.tv_nsec == value.tv_nsec);
    }

    SECTION("serialize current time") {
        std::timespec value{};
        REQUIRE(std::timespec_get(&value, TIME_UTC) == TIME_UTC);

        const binary serialized = serialize(value);

        const msgpack::object_handle deserialized = msgpack::unpack(
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            reinterpret_cast<const char*>(serialized.data()),
            serialized.size());

        std::timespec deserialized_value{};
        REQUIRE_NOTHROW(deserialized_value = deserialized->as<std::timespec>());
        CHECK(deserialized_value.tv_sec == value.tv_sec);
        CHECK(deserialized_value.tv_nsec == value.tv_nsec);
    }
}
