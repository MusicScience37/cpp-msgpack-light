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
 * \brief Test of classes to support serialization of integer values.
 */
#include "msgpack_light/type_support/integer.h"

#include <cstdint>
#include <tuple>

#include <catch2/catch_message.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "msgpack_light/binary.h"
#include "msgpack_light/memory_output_stream.h"
#include "msgpack_light/serialization_buffer.h"

TEMPLATE_TEST_CASE(
    "msgpack_light::type_support::serialization_traits for unsigned integers",
    "", std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t) {
    using msgpack_light::binary;
    using msgpack_light::memory_output_stream;
    using msgpack_light::serialization_buffer;
    using integer_type = TestType;

    SECTION("serialize values from 0x00 to 0xFF") {
        integer_type value{};
        binary expected_binary{};
        std::tie(value, expected_binary) = GENERATE(table<integer_type, binary>(
            {{static_cast<integer_type>(0x00), binary("00")},
                {static_cast<integer_type>(0x39), binary("39")},
                {static_cast<integer_type>(0x7F), binary("7F")},
                {static_cast<integer_type>(0x80), binary("CC80")},
                {static_cast<integer_type>(0xB3), binary("CCB3")},
                // cspell: ignore CCFF
                {static_cast<integer_type>(0xFF), binary("CCFF")}}));
        INFO("value = " << value);

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    if constexpr (sizeof(integer_type) >= 2U) {
        SECTION("serialize values from 0x0100 to 0xFFFF") {
            integer_type value{};
            binary expected_binary{};
            std::tie(value, expected_binary) = GENERATE(
                table<integer_type, binary>({{static_cast<integer_type>(0x100),
                                                 binary("CD0100")},
                    {static_cast<integer_type>(0x3039), binary("CD3039")},
                    // cspell: ignore CDFFFF
                    {static_cast<integer_type>(0xFFFF), binary("CDFFFF")}}));
            INFO("value = " << value);

            memory_output_stream stream;
            serialization_buffer buffer(stream);

            buffer.serialize(value);

            buffer.flush();
            CHECK(stream.as_binary() == expected_binary);
        }
    }

    if constexpr (sizeof(integer_type) >= 4U) {
        SECTION("serialize values from 0x00010000 to 0xFFFFFFFF") {
            integer_type value{};
            binary expected_binary{};
            std::tie(value, expected_binary) =
                GENERATE(table<integer_type, binary>(
                    {{static_cast<integer_type>(0x00010000),
                         binary("CE00010000")},
                        {static_cast<integer_type>(0x075BCD15),
                            binary("CE075BCD15")},
                        // cspell: ignore CEFFFFFFFF
                        {static_cast<integer_type>(0xFFFFFFFF),
                            binary("CEFFFFFFFF")}}));
            INFO("value = " << value);

            memory_output_stream stream;
            serialization_buffer buffer(stream);

            buffer.serialize(value);

            buffer.flush();
            CHECK(stream.as_binary() == expected_binary);
        }
    }

    // NOLINTNEXTLINE(readability-magic-numbers)
    if constexpr (sizeof(integer_type) >= 8U) {
        SECTION(
            "serialize values from 0x0000000100000000 to 0xFFFFFFFFFFFFFFFF") {
            integer_type value{};
            binary expected_binary{};
            std::tie(value, expected_binary) =
                GENERATE(table<integer_type, binary>(
                    {{static_cast<integer_type>(0x0000000100000000),
                         binary("CF0000000100000000")},
                        {static_cast<integer_type>(0x112210F47DE98115),
                            binary("CF112210F47DE98115")},
                        // cspell: ignore CFFFFFFFFFFFFFFFFF
                        {static_cast<integer_type>(0xFFFFFFFFFFFFFFFF),
                            binary("CFFFFFFFFFFFFFFFFF")}}));
            INFO("value = " << value);

            memory_output_stream stream;
            serialization_buffer buffer(stream);

            buffer.serialize(value);

            buffer.flush();
            CHECK(stream.as_binary() == expected_binary);
        }
    }
}

TEMPLATE_TEST_CASE(
    "msgpack_light::type_support::serialization_traits for signed integers", "",
    std::int8_t, std::int16_t, std::int32_t, std::int64_t) {
    using msgpack_light::binary;
    using msgpack_light::memory_output_stream;
    using msgpack_light::serialization_buffer;
    using integer_type = TestType;

    SECTION("serialize values from 0x00 to 0x7F") {
        integer_type value{};
        binary expected_binary{};
        std::tie(value, expected_binary) = GENERATE(table<integer_type, binary>(
            {{static_cast<integer_type>(0x00), binary("00")},
                {static_cast<integer_type>(0x39), binary("39")},
                {static_cast<integer_type>(0x7F), binary("7F")}}));
        INFO("value = " << value);

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    if constexpr (sizeof(integer_type) >= 2U) {
        SECTION("serialize values from 0x0080 to 0x7FFF") {
            integer_type value{};
            binary expected_binary{};
            std::tie(value, expected_binary) = GENERATE(
                table<integer_type, binary>({{static_cast<integer_type>(0x80),
                                                 binary("CC80")},
                    {static_cast<integer_type>(0xB3), binary("CCB3")},
                    // cspell: ignore CCFF
                    {static_cast<integer_type>(0xFF), binary("CCFF")},
                    {static_cast<integer_type>(0x100), binary("CD0100")},
                    {static_cast<integer_type>(0x3039), binary("CD3039")},
                    {static_cast<integer_type>(0x7FFF), binary("CD7FFF")}}));
            INFO("value = " << value);

            memory_output_stream stream;
            serialization_buffer buffer(stream);

            buffer.serialize(value);

            buffer.flush();
            CHECK(stream.as_binary() == expected_binary);
        }
    }

    if constexpr (sizeof(integer_type) >= 4U) {
        SECTION("serialize values from 0x00008000 to 0x7FFFFFFF") {
            integer_type value{};
            binary expected_binary{};
            std::tie(value, expected_binary) = GENERATE(table<integer_type,
                binary>({{static_cast<integer_type>(0x00008000),
                             binary("CD8000")},
                {static_cast<integer_type>(0x0000FFFF), binary("CDFFFF")},
                {static_cast<integer_type>(0x00010000), binary("CE00010000")},
                {static_cast<integer_type>(0x075BCD15), binary("CE075BCD15")},
                {static_cast<integer_type>(0x7FFFFFFF),
                    binary("CE7FFFFFFF")}}));
            INFO("value = " << value);

            memory_output_stream stream;
            serialization_buffer buffer(stream);

            buffer.serialize(value);

            buffer.flush();
            CHECK(stream.as_binary() == expected_binary);
        }
    }

    // NOLINTNEXTLINE(readability-magic-numbers)
    if constexpr (sizeof(integer_type) >= 8U) {
        SECTION(
            "serialize values from 0x0000000080000000 to 0x7FFFFFFFFFFFFFFF") {
            integer_type value{};
            binary expected_binary{};
            std::tie(value, expected_binary) =
                GENERATE(table<integer_type, binary>(
                    {{static_cast<integer_type>(0x0000000080000000),
                         binary("CE80000000")},
                        {static_cast<integer_type>(0x0000000100000000),
                            binary("CF0000000100000000")},
                        {static_cast<integer_type>(0x112210F47DE98115),
                            binary("CF112210F47DE98115")},
                        {static_cast<integer_type>(0x7FFFFFFFFFFFFFFF),
                            binary("CF7FFFFFFFFFFFFFFF")}}));
            INFO("value = " << value);

            memory_output_stream stream;
            serialization_buffer buffer(stream);

            buffer.serialize(value);

            buffer.flush();
            CHECK(stream.as_binary() == expected_binary);
        }
    }

    SECTION("serialize values from 0xFF to 0x80") {
        static const auto create_value = [](auto value) {
            return static_cast<integer_type>(static_cast<std::int8_t>(value));
        };
        integer_type value{};
        binary expected_binary{};
        std::tie(value, expected_binary) = GENERATE(
            table<integer_type, binary>({{create_value(0xFF), binary("FF")},
                {create_value(0xF3), binary("F3")},
                {create_value(0xE0), binary("E0")},
                {create_value(0xDF), binary("D0DF")},
                {create_value(0x9E), binary("D09E")},
                {create_value(0x80), binary("D080")}}));
        INFO("value = " << value);

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    if constexpr (sizeof(integer_type) >= 2U) {
        SECTION("serialize values from 0xFF7F to 0x8000") {
            static const auto create_value = [](auto value) {
                return static_cast<integer_type>(
                    static_cast<std::int16_t>(value));
            };
            integer_type value{};
            binary expected_binary{};
            std::tie(value, expected_binary) =
                GENERATE(table<integer_type, binary>(
                    {{create_value(0xFF7F), binary("D1FF7F")},
                        {create_value(0xCFC7), binary("D1CFC7")},
                        {create_value(0x8000), binary("D18000")}}));
            INFO("value = " << value);

            memory_output_stream stream;
            serialization_buffer buffer(stream);

            buffer.serialize(value);

            buffer.flush();
            CHECK(stream.as_binary() == expected_binary);
        }
    }

    if constexpr (sizeof(integer_type) >= 4U) {
        SECTION("serialize values from 0xFFFF7FFF to 0x80000000") {
            static const auto create_value = [](auto value) {
                return static_cast<integer_type>(
                    static_cast<std::int32_t>(value));
            };
            integer_type value{};
            binary expected_binary{};
            std::tie(value, expected_binary) =
                GENERATE(table<integer_type, binary>({
                    {create_value(0xFFFF7FFF), binary("D2FFFF7FFF")},
                    {create_value(0xB669FD2E), binary("D2B669FD2E")},
                    {create_value(0x80000000), binary("D280000000")},
                }));
            INFO("value = " << value);

            memory_output_stream stream;
            serialization_buffer buffer(stream);

            buffer.serialize(value);

            buffer.flush();
            CHECK(stream.as_binary() == expected_binary);
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
            integer_type value{};
            binary expected_binary{};
            std::tie(value, expected_binary) =
                GENERATE(table<integer_type, binary>({
                    {create_value(0xFFFFFFFF7FFFFFFF),
                        binary("D3FFFFFFFF7FFFFFFF")},
                    {create_value(0xEEDDEF0B82167EEB),
                        binary("D3EEDDEF0B82167EEB")},
                    {create_value(0x8000000000000000),
                        binary("D38000000000000000")},
                }));
            INFO("value = " << value);

            memory_output_stream stream;
            serialization_buffer buffer(stream);

            buffer.serialize(value);

            buffer.flush();
            CHECK(stream.as_binary() == expected_binary);
        }
    }
}
