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
 * \brief Test to serialize integers.
 */
#include <cstdint>
#include <limits>

#include <catch2/catch_message.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <msgpack.hpp>

#include "msgpack_light/binary.h"
#include "msgpack_light/serialize.h"

TEMPLATE_TEST_CASE("serialize unsigned integers", "", std::uint8_t,
    std::uint16_t, std::uint32_t, std::uint64_t) {
    using msgpack_light::binary;
    using msgpack_light::serialize;
    using integer_type = TestType;

    SECTION("serialize values from 0x00 to 0xFF") {
        static const auto create_value = [](auto value) {
            return static_cast<integer_type>(static_cast<std::uint8_t>(value));
        };
        const integer_type value =
            GENERATE(create_value(0x00), create_value(0x39), create_value(0x7F),
                create_value(0x80), create_value(0xB3), create_value(0xFF));
        INFO("value = " << value);

        const binary serialized = serialize(value);

        const msgpack::object_handle deserialized =
            msgpack::unpack(reinterpret_cast<const char*>(serialized.data()),
                serialized.size());

        CHECK(deserialized->as<integer_type>() == value);
    }

    if constexpr (sizeof(integer_type) >= 2U) {
        SECTION("serialize values from 0x0100 to 0xFFFF") {
            static const auto create_value = [](auto value) {
                return static_cast<integer_type>(
                    static_cast<std::uint16_t>(value));
            };
            const integer_type value = GENERATE(create_value(0x100),
                create_value(0x3039), create_value(0xFFFF));
            INFO("value = " << value);

            const binary serialized = serialize(value);

            const msgpack::object_handle deserialized = msgpack::unpack(
                reinterpret_cast<const char*>(serialized.data()),
                serialized.size());

            CHECK(deserialized->as<integer_type>() == value);
        }
    }

    if constexpr (sizeof(integer_type) >= 4U) {
        SECTION("serialize values from 0x00010000 to 0xFFFFFFFF") {
            static const auto create_value = [](auto value) {
                return static_cast<integer_type>(
                    static_cast<std::uint32_t>(value));
            };
            const integer_type value = GENERATE(create_value(0x00010000),
                create_value(0x075BCD15), create_value(0xFFFFFFFF));
            INFO("value = " << value);

            const binary serialized = serialize(value);

            const msgpack::object_handle deserialized = msgpack::unpack(
                reinterpret_cast<const char*>(serialized.data()),
                serialized.size());

            CHECK(deserialized->as<integer_type>() == value);
        }
    }

    // NOLINTNEXTLINE(readability-magic-numbers)
    if constexpr (sizeof(integer_type) >= 8U) {
        SECTION(
            "serialize values from 0x0000000100000000 to 0xFFFFFFFFFFFFFFFF") {
            static const auto create_value = [](auto value) {
                return static_cast<integer_type>(
                    static_cast<std::uint64_t>(value));
            };
            const integer_type value =
                GENERATE(create_value(0x0000000100000000),
                    create_value(0x112210F47DE98115),
                    create_value(0xFFFFFFFFFFFFFFFF));
            INFO("value = " << value);

            const binary serialized = serialize(value);

            const msgpack::object_handle deserialized = msgpack::unpack(
                reinterpret_cast<const char*>(serialized.data()),
                serialized.size());

            CHECK(deserialized->as<integer_type>() == value);
        }
    }

    SECTION("serialize random values") {
        const integer_type value = GENERATE(take(1000,
            random(std::numeric_limits<integer_type>::min(),
                std::numeric_limits<integer_type>::max())));
        INFO("value = " << value);

        const binary serialized = serialize(value);

        const msgpack::object_handle deserialized =
            msgpack::unpack(reinterpret_cast<const char*>(serialized.data()),
                serialized.size());

        CHECK(deserialized->as<integer_type>() == value);
    }
}

TEMPLATE_TEST_CASE("serialize signed integers", "", std::int8_t, std::int16_t,
    std::int32_t, std::int64_t) {
    using msgpack_light::binary;
    using msgpack_light::serialize;
    using integer_type = TestType;

    SECTION("serialize values from 0x00 to 0x7F") {
        static const auto create_value = [](auto value) {
            return static_cast<integer_type>(static_cast<std::int8_t>(value));
        };
        const integer_type value = GENERATE(
            create_value(0x00), create_value(0x39), create_value(0x7F));
        INFO("value = " << value);

        const binary serialized = serialize(value);

        const msgpack::object_handle deserialized =
            msgpack::unpack(reinterpret_cast<const char*>(serialized.data()),
                serialized.size());

        CHECK(deserialized->as<integer_type>() == value);
    }

    if constexpr (sizeof(integer_type) >= 2U) {
        SECTION("serialize values from 0x0080 to 0x7FFF") {
            static const auto create_value = [](auto value) {
                return static_cast<integer_type>(
                    static_cast<std::int16_t>(value));
            };
            const integer_type value = GENERATE(create_value(0x80),
                create_value(0xB3), create_value(0xFF), create_value(0x100),
                create_value(0x3039), create_value(0x7FFF));
            INFO("value = " << value);

            const binary serialized = serialize(value);

            const msgpack::object_handle deserialized = msgpack::unpack(
                reinterpret_cast<const char*>(serialized.data()),
                serialized.size());

            CHECK(deserialized->as<integer_type>() == value);
        }
    }

    if constexpr (sizeof(integer_type) >= 4U) {
        SECTION("serialize values from 0x00008000 to 0x7FFFFFFF") {
            static const auto create_value = [](auto value) {
                return static_cast<integer_type>(
                    static_cast<std::int32_t>(value));
            };
            const integer_type value = GENERATE(create_value(0x00008000),
                create_value(0x0000FFFF), create_value(0x00010000),
                create_value(0x075BCD15), create_value(0x7FFFFFFF));
            INFO("value = " << value);

            const binary serialized = serialize(value);

            const msgpack::object_handle deserialized = msgpack::unpack(
                reinterpret_cast<const char*>(serialized.data()),
                serialized.size());

            CHECK(deserialized->as<integer_type>() == value);
        }
    }

    // NOLINTNEXTLINE(readability-magic-numbers)
    if constexpr (sizeof(integer_type) >= 8U) {
        SECTION(
            "serialize values from 0x0000000080000000 to 0x7FFFFFFFFFFFFFFF") {
            static const auto create_value = [](auto value) {
                return static_cast<integer_type>(
                    static_cast<std::int64_t>(value));
            };
            const integer_type value =
                GENERATE(create_value(0x0000000080000000),
                    create_value(0x0000000100000000),
                    create_value(0x112210F47DE98115),
                    create_value(0x7FFFFFFFFFFFFFFF));
            INFO("value = " << value);

            const binary serialized = serialize(value);

            const msgpack::object_handle deserialized = msgpack::unpack(
                reinterpret_cast<const char*>(serialized.data()),
                serialized.size());

            CHECK(deserialized->as<integer_type>() == value);
        }
    }

    SECTION("serialize values from 0xFF to 0x80") {
        static const auto create_value = [](auto value) {
            return static_cast<integer_type>(static_cast<std::int8_t>(value));
        };
        const integer_type value =
            GENERATE(create_value(0xFF), create_value(0xF3), create_value(0xE0),
                create_value(0xDF), create_value(0x9E), create_value(0x80));
        INFO("value = " << value);

        const binary serialized = serialize(value);

        const msgpack::object_handle deserialized =
            msgpack::unpack(reinterpret_cast<const char*>(serialized.data()),
                serialized.size());

        CHECK(deserialized->as<integer_type>() == value);
    }

    if constexpr (sizeof(integer_type) >= 2U) {
        SECTION("serialize values from 0xFF7F to 0x8000") {
            static const auto create_value = [](auto value) {
                return static_cast<integer_type>(
                    static_cast<std::int16_t>(value));
            };
            const integer_type value = GENERATE(create_value(0xFF7F),
                create_value(0xCFC7), create_value(0x8000));
            INFO("value = " << value);

            const binary serialized = serialize(value);

            const msgpack::object_handle deserialized = msgpack::unpack(
                reinterpret_cast<const char*>(serialized.data()),
                serialized.size());

            CHECK(deserialized->as<integer_type>() == value);
        }
    }

    if constexpr (sizeof(integer_type) >= 4U) {
        SECTION("serialize values from 0xFFFF7FFF to 0x80000000") {
            static const auto create_value = [](auto value) {
                return static_cast<integer_type>(
                    static_cast<std::int32_t>(value));
            };
            const integer_type value = GENERATE(create_value(0xFFFF7FFF),
                create_value(0xB669FD2E), create_value(0x80000000));
            INFO("value = " << value);

            const binary serialized = serialize(value);

            const msgpack::object_handle deserialized = msgpack::unpack(
                reinterpret_cast<const char*>(serialized.data()),
                serialized.size());

            CHECK(deserialized->as<integer_type>() == value);
        }
    }

    // NOLINTNEXTLINE(readability-magic-numbers)
    if constexpr (sizeof(integer_type) >= 8U) {
        SECTION(
            "serialize values from 0xFFFFFFFF7FFFFFFF to 0x8000000000000000") {
            static const auto create_value = [](auto value) {
                return static_cast<integer_type>(
                    static_cast<std::int64_t>(value));
            };
            const integer_type value =
                GENERATE(create_value(0xFFFFFFFF7FFFFFFF),
                    create_value(0xEEDDEF0B82167EEB),
                    create_value(0x8000000000000000));
            INFO("value = " << value);

            const binary serialized = serialize(value);

            const msgpack::object_handle deserialized = msgpack::unpack(
                reinterpret_cast<const char*>(serialized.data()),
                serialized.size());

            CHECK(deserialized->as<integer_type>() == value);
        }
    }

    SECTION("serialize random values") {
        const integer_type value = GENERATE(take(1000,
            random(std::numeric_limits<integer_type>::min(),
                std::numeric_limits<integer_type>::max())));
        INFO("value = " << value);

        const binary serialized = serialize(value);

        const msgpack::object_handle deserialized =
            msgpack::unpack(reinterpret_cast<const char*>(serialized.data()),
                serialized.size());

        CHECK(deserialized->as<integer_type>() == value);
    }
}
