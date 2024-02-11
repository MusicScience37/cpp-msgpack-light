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
 * \brief Test of classes to support serialization of floating-point
 * numbers.
 */
#include "msgpack_light/type_support/float.h"

#include <catch2/catch_test_macros.hpp>

#include "msgpack_light/binary.h"
#include "msgpack_light/memory_output_stream.h"
#include "msgpack_light/serialization_buffer.h"

TEST_CASE("msgpack_light::type_support::serialization_traits<float>") {
    using msgpack_light::binary;
    using msgpack_light::memory_output_stream;
    using msgpack_light::serialization_buffer;

    SECTION("serialize") {
        constexpr float value = 0x1.23456p+0F;
        const auto expected_binary = binary("CA3F91A2B0");

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }
}

TEST_CASE("msgpack_light::type_support::serialization_traits<double>") {
    using msgpack_light::binary;
    using msgpack_light::memory_output_stream;
    using msgpack_light::serialization_buffer;

    SECTION("serialize") {
        constexpr double value = 0x1.23456789ABCDEp+0;
        const auto expected_binary = binary("CB3FF23456789ABCDE");

        memory_output_stream stream;
        serialization_buffer buffer(stream);

        buffer.serialize(value);

        buffer.flush();
        CHECK(stream.as_binary() == expected_binary);
    }
}
