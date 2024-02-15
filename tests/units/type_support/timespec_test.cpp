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
 * \brief Test of classes to support serialization of std::timespec
 * objects.
 */
#include "msgpack_light/type_support/timespec.h"

#include <ctime>

#include <catch2/catch_test_macros.hpp>

#include "msgpack_light/binary.h"
#include "msgpack_light/memory_output_stream.h"
#include "msgpack_light/serialization_buffer.h"

TEST_CASE("msgpack_light::type_support::serialization_traits<std::timespec>") {
    using msgpack_light::binary;
    using msgpack_light::memory_output_stream;
    using msgpack_light::serialization_buffer;

    SECTION("serialize timestamp 32") {
        std::timespec value{};
        value.tv_sec = static_cast<std::time_t>(0xABCD1234U);  // NOLINT
        value.tv_nsec = 0;
        const auto expected_binary = binary("D6FFABCD1234");

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize timestamp 64") {
        std::timespec value{};
        value.tv_sec = static_cast<std::time_t>(0x2ABCD1234U);  // NOLINT
        value.tv_nsec = 0x75BCD15;                              // NOLINT
        const auto expected_binary = binary("D7FF1D6F3456ABCD1234");

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }

    SECTION("serialize timestamp 96") {
        std::timespec value{};
        value.tv_sec = static_cast<std::time_t>(0x123456789ABCDEF1U);  // NOLINT
        value.tv_nsec = 0x75BCD15;                                     // NOLINT
        const auto expected_binary = binary("C70CFF075BCD15123456789ABCDEF1");

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }
}
