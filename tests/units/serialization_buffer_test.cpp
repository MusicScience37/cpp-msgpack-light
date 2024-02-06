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

#include "msgpack_light/memory_output_stream.h"
#include "msgpack_light_test/binary.h"

TEST_CASE("msgpack_light::serialization_buffer") {
    using msgpack_light::memory_output_stream;
    using msgpack_light::serialization_buffer;
    using msgpack_light_test::binary;

    SECTION("serialize bool") {
        SECTION("serialize value `false`") {
            memory_output_stream stream;
            serialization_buffer buffer(stream);

            buffer.serialize_bool(false);

            CHECK(binary(stream.data(), stream.size()) == binary("C2"));
        }

        SECTION("serialize value `true`") {
            memory_output_stream stream;
            serialization_buffer buffer(stream);

            buffer.serialize_bool(true);

            CHECK(binary(stream.data(), stream.size()) == binary("C3"));
        }
    }

    SECTION("serialize positive fixint") {
        SECTION("serialize value `0`") {
            memory_output_stream stream;
            serialization_buffer buffer(stream);

            constexpr auto value = static_cast<std::uint8_t>(0);
            buffer.serialize_positive_fixint(value);

            CHECK(binary(stream.data(), stream.size()) == binary("00"));
        }

        SECTION("serialize value `57`") {
            memory_output_stream stream;
            serialization_buffer buffer(stream);

            constexpr auto value = static_cast<std::uint8_t>(57);
            buffer.serialize_positive_fixint(value);

            CHECK(binary(stream.data(), stream.size()) == binary("39"));
        }

        SECTION("serialize value `0x7F`") {
            memory_output_stream stream;
            serialization_buffer buffer(stream);

            constexpr auto value = static_cast<std::uint8_t>(0x7F);
            buffer.serialize_positive_fixint(value);

            CHECK(binary(stream.data(), stream.size()) == binary("7F"));
        }
    }

    SECTION("serialize negative fixint") {
        SECTION("serialize value `-1`") {
            memory_output_stream stream;
            serialization_buffer buffer(stream);

            constexpr auto value = static_cast<std::int8_t>(-1);
            buffer.serialize_negative_fixint(value);

            CHECK(binary(stream.data(), stream.size()) == binary("FF"));
        }

        SECTION("serialize value `-13`") {
            memory_output_stream stream;
            serialization_buffer buffer(stream);

            constexpr auto value = static_cast<std::int8_t>(-13);
            buffer.serialize_negative_fixint(value);

            CHECK(binary(stream.data(), stream.size()) == binary("F3"));
        }

        SECTION("serialize value `0xE0`") {
            memory_output_stream stream;
            serialization_buffer buffer(stream);

            constexpr auto value = static_cast<std::int8_t>(0xE0);
            buffer.serialize_negative_fixint(value);

            CHECK(binary(stream.data(), stream.size()) == binary("E0"));
        }
    }

    SECTION("format uint 8") {
        SECTION("serialize `0x80") {
            memory_output_stream stream;
            serialization_buffer buffer(stream);

            constexpr auto value = static_cast<std::uint8_t>(0x80);
            buffer.serialize_uint8(value);

            CHECK(binary(stream.data(), stream.size()) == binary("CC80"));
        }

        SECTION("serialize `176") {
            memory_output_stream stream;
            serialization_buffer buffer(stream);

            constexpr auto value = static_cast<std::uint8_t>(0xB0);
            buffer.serialize_uint8(value);

            CHECK(binary(stream.data(), stream.size()) == binary("CCB0"));
        }

        SECTION("serialize `0xFF") {
            memory_output_stream stream;
            serialization_buffer buffer(stream);

            constexpr auto value = static_cast<std::uint8_t>(0xFF);
            buffer.serialize_uint8(value);

            // cspell: ignore CCFF
            CHECK(binary(stream.data(), stream.size()) == binary("CCFF"));
        }
    }

    SECTION("serialize uint 16") {
        SECTION("serialize `0x100`") {
            memory_output_stream stream;
            serialization_buffer buffer(stream);

            constexpr auto value = static_cast<std::uint16_t>(0x100);
            buffer.serialize_uint16(value);

            CHECK(binary(stream.data(), stream.size()) == binary("CD0100"));
        }

        SECTION("serialize `12345`") {
            memory_output_stream stream;
            serialization_buffer buffer(stream);

            constexpr auto value = static_cast<std::uint16_t>(12345);
            buffer.serialize_uint16(value);

            CHECK(binary(stream.data(), stream.size()) == binary("CD3039"));
        }

        SECTION("serialize `0xFFFF`") {
            memory_output_stream stream;
            serialization_buffer buffer(stream);

            constexpr auto value = static_cast<std::uint16_t>(0xFFFF);
            buffer.serialize_uint16(value);

            // cspell: ignore CDFFFF
            CHECK(binary(stream.data(), stream.size()) == binary("CDFFFF"));
        }
    }
}
