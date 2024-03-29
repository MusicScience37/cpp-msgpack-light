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
 * \brief Test of buffered_serialization_buffer_impl class.
 */
#include "msgpack_light/details/buffered_serialization_buffer_impl.h"

#include <cstdint>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "msgpack_light/binary.h"
#include "msgpack_light/details/static_memory_buffer_size.h"
#include "msgpack_light/memory_output_stream.h"

TEST_CASE("msgpack_light::details::buffered_serialization_buffer_impl") {
    using msgpack_light::binary;
    using msgpack_light::memory_output_stream;
    using msgpack_light::details::buffered_serialization_buffer_impl;
    using msgpack_light::details::static_memory_buffer_size;

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
        buffered_serialization_buffer_impl buffer(stream);

        buffer.write(data.data(), data.size());

        buffer.flush();
        CHECK(stream.as_binary() == data);
    }

    SECTION("write a byte") {
        const auto data = binary("81");

        memory_output_stream stream;
        buffered_serialization_buffer_impl buffer(stream);

        buffer.put(*data.data());

        buffer.flush();
        CHECK(stream.as_binary() == data);
    }

    SECTION("write a byte when the buffer is full") {
        const auto buffered_data_vec = std::vector<unsigned char>(
            static_memory_buffer_size, static_cast<unsigned char>(0x81));
        const auto buffered_data =
            binary(buffered_data_vec.data(), buffered_data_vec.size());
        const auto data = binary("81");

        memory_output_stream stream;
        buffered_serialization_buffer_impl buffer(stream);
        buffer.write(buffered_data.data(), buffered_data.size());

        // This will flush the buffer.
        buffer.put(*data.data());

        buffer.flush();
        CHECK(stream.as_binary() == buffered_data + data);
    }

    SECTION("write an integer in big endian") {
        memory_output_stream stream;
        buffered_serialization_buffer_impl buffer(stream);

        constexpr auto value = static_cast<std::uint32_t>(0x12345678U);
        buffer.write_in_big_endian(value);

        buffer.flush();
        CHECK(stream.as_binary() == binary("12345678"));
    }

    SECTION("write two integers in big endian") {
        memory_output_stream stream;
        buffered_serialization_buffer_impl buffer(stream);

        constexpr auto value1 = static_cast<std::uint8_t>(0x12);
        constexpr auto value2 = static_cast<std::uint32_t>(0x3456789AU);
        buffer.write_in_big_endian(value1, value2);

        buffer.flush();
        CHECK(stream.as_binary() == binary("123456789A"));
    }

    SECTION("write three integers in big endian") {
        memory_output_stream stream;
        buffered_serialization_buffer_impl buffer(stream);

        constexpr auto value1 = static_cast<std::uint8_t>(0x12);
        constexpr auto value2 = static_cast<std::uint32_t>(0x3456789AU);
        constexpr auto value3 = static_cast<std::uint16_t>(0xBCDE);
        buffer.write_in_big_endian(value1, value2, value3);

        buffer.flush();
        CHECK(stream.as_binary() == binary("123456789ABCDE"));
    }
}
