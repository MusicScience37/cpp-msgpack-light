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
 * \brief Test to serialize values in ext format.
 */
#include <cstddef>
#include <map>
#include <string>

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <catch2/generators/catch_generators_range.hpp>
#include <msgpack.hpp>

#include "msgpack_light/binary.h"
#include "msgpack_light/memory_output_stream.h"
#include "msgpack_light/serialization_buffer_fwd.h"
#include "msgpack_light/serialize.h"

TEST_CASE("serialize values in ext format") {
    using msgpack_light::binary;
    using msgpack_light::memory_output_stream;
    using msgpack_light::serialization_buffer;

    SECTION("serialize values with small data sizes") {
        const auto ext_type = static_cast<std::size_t>(123);
        const std::size_t data_size = GENERATE(
            range(static_cast<std::size_t>(0), static_cast<std::size_t>(300)));
        const auto data = std::vector<unsigned char>(
            data_size, static_cast<unsigned char>(0xA1));

        memory_output_stream stream;
        serialization_buffer buffer{stream};
        buffer.serialize_ext_header(ext_type, data_size);
        buffer.write(data.data(), data.size());
        buffer.flush();
        const binary serialized = stream.as_binary();

        const msgpack::object_handle deserialized = msgpack::unpack(
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            reinterpret_cast<const char*>(serialized.data()),
            serialized.size());
        REQUIRE(deserialized->type == msgpack::type::EXT);
        CHECK(deserialized->via.ext.type() == ext_type);
        REQUIRE(deserialized->via.ext.size == data_size);
    }

    SECTION("serialize values with various data sizes") {
        const auto ext_type = static_cast<std::size_t>(86);
        const std::size_t data_size = GENERATE(take(100,
            random(static_cast<std::size_t>(0),
                static_cast<std::size_t>(0x15000))));
        const auto data = std::vector<unsigned char>(
            data_size, static_cast<unsigned char>(0xA1));

        memory_output_stream stream;
        serialization_buffer buffer{stream};
        buffer.serialize_ext_header(ext_type, data_size);
        buffer.write(data.data(), data.size());
        buffer.flush();
        const binary serialized = stream.as_binary();

        const msgpack::object_handle deserialized = msgpack::unpack(
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            reinterpret_cast<const char*>(serialized.data()),
            serialized.size());
        REQUIRE(deserialized->type == msgpack::type::EXT);
        CHECK(deserialized->via.ext.type() == ext_type);
        REQUIRE(deserialized->via.ext.size == data_size);
    }
}
