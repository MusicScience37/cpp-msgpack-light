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
 * \brief Test of serialization_buffer class.
 */
#include "msgpack_light/serialization_buffer.h"

#include <cstdint>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "msgpack_light/binary.h"
#include "msgpack_light/memory_output_stream.h"

TEST_CASE("msgpack_light::serialization_buffer") {
    using msgpack_light::binary;
    using msgpack_light::memory_output_stream;
    using msgpack_light::serialization_buffer;

    SECTION("serialize nil") {
        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_nil();

        buffer.flush();
        binary expected_binary{"C0"};
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize bool") {
        bool value{};
        binary expected_binary{};
        std::tie(value, expected_binary) = GENERATE(
            table<bool, binary>({{false, binary("C2")}, {true, binary("C3")}}));
        INFO("value = " << value);

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_bool(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize positive fixint") {
        std::uint8_t value{};
        binary expected_binary{};
        std::tie(value, expected_binary) = GENERATE(table<std::uint8_t, binary>(
            {{static_cast<std::uint8_t>(0), binary("00")},
                {static_cast<std::uint8_t>(57), binary("39")},
                {static_cast<std::uint8_t>(0x7F), binary("7F")}}));
        INFO("value = " << value);

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_positive_fixint(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize negative fixint") {
        std::int8_t value{};
        binary expected_binary{};
        std::tie(value, expected_binary) = GENERATE(table<std::int8_t, binary>(
            {{static_cast<std::int8_t>(-1), binary("FF")},
                {static_cast<std::int8_t>(-13), binary("F3")},
                {static_cast<std::int8_t>(0xE0), binary("E0")}}));
        INFO("value = " << value);

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_negative_fixint(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("format uint 8") {
        std::uint8_t value{};
        binary expected_binary{};
        std::tie(value, expected_binary) = GENERATE(table<std::uint8_t, binary>(
            {{static_cast<std::uint8_t>(0x80), binary("CC80")},
                {static_cast<std::uint8_t>(0xB0), binary("CCB0")},
                // cspell: ignore CCFF
                {static_cast<std::uint8_t>(0xFF), binary("CCFF")}}));
        INFO("value = " << value);

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_uint8(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize uint 16") {
        std::uint16_t value{};
        binary expected_binary{};
        std::tie(value, expected_binary) =
            GENERATE(table<std::uint16_t, binary>(
                {{static_cast<std::uint16_t>(0x100), binary("CD0100")},
                    {static_cast<std::uint16_t>(12345), binary("CD3039")},
                    // cspell: ignore CDFFFF
                    {static_cast<std::uint16_t>(0xFFFF), binary("CDFFFF")}}));
        INFO("value = " << value);

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_uint16(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize uint 32") {
        std::uint32_t value{};
        binary expected_binary{};
        std::tie(value, expected_binary) = GENERATE(
            table<std::uint32_t, binary>({{static_cast<std::uint32_t>(0x10000),
                                              binary("CE00010000")},
                {static_cast<std::uint32_t>(123456789), binary("CE075BCD15")},
                // cspell: ignore CEFFFFFFFF
                {static_cast<std::uint32_t>(0xFFFFFFFF),
                    binary("CEFFFFFFFF")}}));
        INFO("value = " << value);

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_uint32(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize uint 64") {
        std::uint64_t value{};
        binary expected_binary{};
        std::tie(value, expected_binary) =
            GENERATE(table<std::uint64_t, binary>(
                {{static_cast<std::uint64_t>(0x100000000),
                     binary("CF0000000100000000")},
                    {static_cast<std::uint64_t>(1234567890123456789),
                        binary("CF112210F47DE98115")},
                    // cspell: ignore CFFFFFFFFFFFFFFFFF
                    {static_cast<std::uint64_t>(0xFFFFFFFFFFFFFFFF),
                        binary("CFFFFFFFFFFFFFFFFF")}}));
        INFO("value = " << value);

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_uint64(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize int 8") {
        std::int8_t value{};
        binary expected_binary{};
        std::tie(value, expected_binary) = GENERATE(table<std::int8_t, binary>(
            {{static_cast<std::int8_t>(0xDF), binary("D0DF")},
                {static_cast<std::int8_t>(-98), binary("D09E")},
                {static_cast<std::int8_t>(0x80), binary("D080")}}));
        INFO("value = " << value);

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_int8(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize int 16") {
        std::int16_t value{};
        binary expected_binary{};
        std::tie(value, expected_binary) = GENERATE(table<std::int16_t, binary>(
            {{static_cast<std::int16_t>(0xFF7F), binary("D1FF7F")},
                {static_cast<std::int16_t>(-12345), binary("D1CFC7")},
                {static_cast<std::int16_t>(0x8000), binary("D18000")}}));
        INFO("value = " << value);

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_int16(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize int 32") {
        std::int32_t value{};
        binary expected_binary{};
        std::tie(value, expected_binary) = GENERATE(table<std::int32_t, binary>(
            {{static_cast<std::int32_t>(0xFFFF7FFF), binary("D2FFFF7FFF")},
                {static_cast<std::int32_t>(-1234567890), binary("D2B669FD2E")},
                {static_cast<std::int32_t>(0x80000000),
                    binary("D280000000")}}));
        INFO("value = " << value);

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_int32(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize int 64") {
        std::int64_t value{};
        binary expected_binary{};
        std::tie(value, expected_binary) = GENERATE(table<std::int64_t, binary>(
            {{static_cast<std::int64_t>(0xFFFFFFFF7FFFFFFF),
                 binary("D3FFFFFFFF7FFFFFFF")},
                {static_cast<std::int64_t>(-1234567890123456789),
                    binary("D3EEDDEF0B82167EEB")},
                {static_cast<std::int64_t>(0x8000000000000000),
                    binary("D38000000000000000")}}));
        INFO("value = " << value);

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_int64(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize float 32") {
        constexpr float value = 0x1.23456p+0F;
        const auto expected_binary = binary("CA3F91A2B0");

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_float32(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize float 64") {
        constexpr double value = 0x1.23456789ABCDEp+0;
        const auto expected_binary = binary("CB3FF23456789ABCDE");

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_float64(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("write data") {
        const std::size_t data_size = GENERATE(static_cast<std::size_t>(0),
            static_cast<std::size_t>(1), static_cast<std::size_t>(1023),
            static_cast<std::size_t>(1024), static_cast<std::size_t>(1025));
        const auto data_vec = std::vector<unsigned char>(
            data_size, static_cast<unsigned char>(0x81));
        const auto data = binary(data_vec.data(), data_vec.size());

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.write(data.data(), data.size());

        buffer.flush();
        CHECK(stream.as_binary() == data);
    }

    SECTION("write a byte") {
        const auto data = binary("81");

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.put(*data.data());

        buffer.flush();
        CHECK(stream.as_binary() == data);
    }
}
