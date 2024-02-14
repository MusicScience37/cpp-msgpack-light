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
#include <tuple>
#include <vector>

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "msgpack_light/binary.h"
#include "msgpack_light/details/static_memory_buffer_size.h"
#include "msgpack_light/memory_output_stream.h"

TEST_CASE("msgpack_light::serialization_buffer") {
    using msgpack_light::binary;
    using msgpack_light::memory_output_stream;
    using msgpack_light::serialization_buffer;
    using msgpack_light::details::static_memory_buffer_size;

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

    SECTION("serialize size of fixstr") {
        std::uint8_t size{};
        binary expected_binary;
        std::tie(size, expected_binary) = GENERATE(table<std::uint8_t, binary>(
            {{static_cast<std::uint8_t>(0x00U), binary("A0")},
                {static_cast<std::uint8_t>(0x01U), binary("A1")},
                {static_cast<std::uint8_t>(0x0AU), binary("AA")},
                {static_cast<std::uint8_t>(0x1FU), binary("BF")}}));

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_fixstr_size(size);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize size of str 8") {
        std::uint8_t size{};
        binary expected_binary;
        std::tie(size, expected_binary) = GENERATE(table<std::uint8_t, binary>(
            {{static_cast<std::uint8_t>(0x20U), binary("D920")},
                {static_cast<std::uint8_t>(0xA7U), binary("D9A7")},
                {static_cast<std::uint8_t>(0xFFU), binary("D9FF")}}));

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_str8_size(size);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize size of str 16") {
        std::uint16_t size{};
        binary expected_binary;
        std::tie(size, expected_binary) = GENERATE(table<std::uint16_t, binary>(
            {{static_cast<std::uint16_t>(0x0100U), binary("DA0100")},
                {static_cast<std::uint16_t>(0x8A54U), binary("DA8A54")},
                // cspell: ignore DAFFFF
                {static_cast<std::uint16_t>(0xFFFFU), binary("DAFFFF")}}));

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_str16_size(size);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize size of str 32") {
        std::uint32_t size{};
        binary expected_binary;
        std::tie(size, expected_binary) = GENERATE(table<std::uint32_t, binary>(
            {{static_cast<std::uint32_t>(0x00010000U), binary("DB00010000")},
                {static_cast<std::uint32_t>(0xA57B531CU), binary("DBA57B531C")},
                // cspell: ignore DBFFFFFFFF
                {static_cast<std::uint32_t>(0xFFFFFFFFU),
                    binary("DBFFFFFFFF")}}));

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_str32_size(size);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize size of strings") {
        std::size_t size{};
        binary expected_binary;
        std::tie(size, expected_binary) = GENERATE(table<std::size_t, binary>(
            {{static_cast<std::size_t>(0x00U), binary("A0")},
                {static_cast<std::size_t>(0x01U), binary("A1")},
                {static_cast<std::size_t>(0x0AU), binary("AA")},
                {static_cast<std::size_t>(0x1FU), binary("BF")},
                {static_cast<std::size_t>(0x20U), binary("D920")},
                {static_cast<std::size_t>(0xA7U), binary("D9A7")},
                {static_cast<std::size_t>(0xFFU), binary("D9FF")},
                {static_cast<std::size_t>(0x0100U), binary("DA0100")},
                {static_cast<std::size_t>(0x8A54U), binary("DA8A54")},
                // cspell: ignore DAFFFF
                {static_cast<std::size_t>(0xFFFFU), binary("DAFFFF")},
                {static_cast<std::size_t>(0x00010000U), binary("DB00010000")},
                {static_cast<std::size_t>(0xA57B531CU), binary("DBA57B531C")},
                // cspell: ignore DBFFFFFFFF
                {static_cast<std::size_t>(0xFFFFFFFFU),
                    binary("DBFFFFFFFF")}}));

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_str_size(size);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    if constexpr (sizeof(std::size_t) > 4U) {
        SECTION("try to serialize too large size of string") {
            memory_output_stream stream;
            serialization_buffer buffer(stream);

            constexpr auto size = static_cast<std::size_t>(0x100000000U);
            CHECK_THROWS(buffer.serialize_str_size(size));
        }
    }

    SECTION("serialize sizes of bin 8") {
        std::uint8_t size{};
        binary expected_binary;
        std::tie(size, expected_binary) = GENERATE(table<std::uint8_t, binary>(
            {{static_cast<std::uint8_t>(0x00U), binary("C400")},
                {static_cast<std::uint8_t>(0xA7U), binary("C4A7")},
                {static_cast<std::uint8_t>(0xFFU), binary("C4FF")}}));

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_bin8_size(size);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize sizes of bin 16") {
        std::uint16_t size{};
        binary expected_binary;
        std::tie(size, expected_binary) = GENERATE(table<std::uint16_t, binary>(
            {{static_cast<std::uint16_t>(0x0100U), binary("C50100")},
                {static_cast<std::uint16_t>(0x8A54U), binary("C58A54")},
                {static_cast<std::uint16_t>(0xFFFFU), binary("C5FFFF")}}));

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_bin16_size(size);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize sizes of bin 32") {
        std::uint32_t size{};
        binary expected_binary;
        std::tie(size, expected_binary) = GENERATE(table<std::uint32_t, binary>(
            {{static_cast<std::uint32_t>(0x00010000U), binary("C600010000")},
                {static_cast<std::uint32_t>(0xA57B531CU), binary("C6A57B531C")},
                {static_cast<std::uint32_t>(0xFFFFFFFFU),
                    binary("C6FFFFFFFF")}}));

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_bin32_size(size);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize size of binaries") {
        std::size_t size{};
        binary expected_binary;
        std::tie(size, expected_binary) = GENERATE(table<std::size_t, binary>(
            {{static_cast<std::size_t>(0x00U), binary("C400")},
                {static_cast<std::size_t>(0xA7U), binary("C4A7")},
                {static_cast<std::size_t>(0xFFU), binary("C4FF")},
                {static_cast<std::size_t>(0x0100U), binary("C50100")},
                {static_cast<std::size_t>(0x8A54U), binary("C58A54")},
                {static_cast<std::size_t>(0xFFFFU), binary("C5FFFF")},
                {static_cast<std::size_t>(0x00010000U), binary("C600010000")},
                {static_cast<std::size_t>(0xA57B531CU), binary("C6A57B531C")},
                {static_cast<std::size_t>(0xFFFFFFFFU),
                    binary("C6FFFFFFFF")}}));

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_bin_size(size);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    if constexpr (sizeof(std::size_t) > 4U) {
        SECTION("try to serialize too large size of binary") {
            memory_output_stream stream;
            serialization_buffer buffer(stream);

            constexpr auto size = static_cast<std::size_t>(0x100000000U);
            CHECK_THROWS(buffer.serialize_bin_size(size));
        }
    }

    SECTION("serialize size of fixarray") {
        std::uint8_t size{};
        binary expected_binary;
        std::tie(size, expected_binary) = GENERATE(table<std::uint8_t, binary>(
            {{static_cast<std::uint8_t>(0x00U), binary("90")},
                {static_cast<std::uint8_t>(0x07U), binary("97")},
                {static_cast<std::uint8_t>(0x0FU), binary("9F")}}));

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_fixarray_size(size);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize size of array 16") {
        std::uint16_t size{};
        binary expected_binary;
        std::tie(size, expected_binary) = GENERATE(table<std::uint16_t, binary>(
            {{static_cast<std::uint16_t>(0x0010U), binary("DC0010")},
                {static_cast<std::uint16_t>(0x1324), binary("DC1324")},
                // cspell: ignore DCFFFF
                {static_cast<std::uint16_t>(0xFFFFU), binary("DCFFFF")}}));

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_array16_size(size);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize size of array 32") {
        std::uint32_t size{};
        binary expected_binary;
        std::tie(size, expected_binary) = GENERATE(table<std::uint32_t, binary>(
            {{static_cast<std::uint32_t>(0x00010000U), binary("DD00010000")},
                {static_cast<std::uint32_t>(0x12345678), binary("DD12345678")},
                // cspell: ignore DDFFFFFFFF
                {static_cast<std::uint32_t>(0xFFFFFFFFU),
                    binary("DDFFFFFFFF")}}));

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_array32_size(size);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize sizes of arrays") {
        std::size_t size{};
        binary expected_binary;
        std::tie(size, expected_binary) = GENERATE(table<std::size_t, binary>(
            {{static_cast<std::size_t>(0x00U), binary("90")},
                {static_cast<std::size_t>(0x07U), binary("97")},
                {static_cast<std::size_t>(0x0FU), binary("9F")},
                {static_cast<std::size_t>(0x0010U), binary("DC0010")},
                {static_cast<std::size_t>(0x1324), binary("DC1324")},
                {static_cast<std::size_t>(0xFFFFU), binary("DCFFFF")},
                {static_cast<std::size_t>(0x00010000U), binary("DD00010000")},
                {static_cast<std::size_t>(0x12345678), binary("DD12345678")},
                {static_cast<std::size_t>(0xFFFFFFFFU),
                    binary("DDFFFFFFFF")}}));

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_array_size(size);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    if constexpr (sizeof(std::size_t) > 4U) {
        SECTION("try to serialize too large size of array") {
            memory_output_stream stream;
            serialization_buffer buffer(stream);

            constexpr auto size = static_cast<std::size_t>(0x100000000U);
            CHECK_THROWS(buffer.serialize_array_size(size));
        }
    }

    SECTION("serialize size of fixmap") {
        std::uint8_t size{};
        binary expected_binary;
        std::tie(size, expected_binary) = GENERATE(table<std::uint8_t, binary>(
            {{static_cast<std::uint8_t>(0x00U), binary("80")},
                {static_cast<std::uint8_t>(0x07U), binary("87")},
                {static_cast<std::uint8_t>(0x0FU), binary("8F")}}));

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_fixmap_size(size);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize size of map 16") {
        std::uint16_t size{};
        binary expected_binary;
        std::tie(size, expected_binary) = GENERATE(table<std::uint16_t, binary>(
            {{static_cast<std::uint16_t>(0x0010U), binary("DE0010")},
                {static_cast<std::uint16_t>(0x1324), binary("DE1324")},
                // cspell: ignore DEFFFF
                {static_cast<std::uint16_t>(0xFFFFU), binary("DEFFFF")}}));

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_map16_size(size);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize size of map 32") {
        std::uint32_t size{};
        binary expected_binary;
        std::tie(size, expected_binary) = GENERATE(table<std::uint32_t, binary>(
            {{static_cast<std::uint32_t>(0x00010000U), binary("DF00010000")},
                {static_cast<std::uint32_t>(0x12345678), binary("DF12345678")},
                // cspell: ignore DFFFFFFFFF
                {static_cast<std::uint32_t>(0xFFFFFFFFU),
                    binary("DFFFFFFFFF")}}));

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize_map32_size(size);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("write data") {
        const std::size_t data_size =
            GENERATE(static_cast<std::size_t>(0), static_cast<std::size_t>(1),
                static_cast<std::size_t>(static_memory_buffer_size - 1U),
                static_cast<std::size_t>(static_memory_buffer_size),
                static_cast<std::size_t>(static_memory_buffer_size + 1U));
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
