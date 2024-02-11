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
 * \brief Test of to_big_endian function.
 */
#include "msgpack_light/details/to_big_endian.h"

#include <array>
#include <cstdint>

#include <catch2/catch_test_macros.hpp>

#include "msgpack_light/binary.h"

TEST_CASE("msgpack_light::details::to_big_endian") {
    using msgpack_light::binary;
    using msgpack_light::details::to_big_endian;

    SECTION("convert 2 byte integer") {
        std::array<unsigned char, 2U> buffer{};

        constexpr auto value = static_cast<std::uint16_t>(0x1234U);
        to_big_endian(&value, &buffer);

        CHECK(binary(buffer.data(), buffer.size()) == binary("1234"));
    }

    SECTION("convert 4 byte integer") {
        std::array<unsigned char, 4U> buffer{};

        constexpr auto value = static_cast<std::uint32_t>(0x12345678U);
        to_big_endian(&value, &buffer);

        CHECK(binary(buffer.data(), buffer.size()) == binary("12345678"));
    }

    SECTION("convert 8 byte integer") {
        // NOLINTNEXTLINE
        std::array<unsigned char, 8U> buffer{};

        constexpr auto value = static_cast<std::uint64_t>(0x123456789ABCDEF1U);
        to_big_endian(&value, &buffer);

        CHECK(
            binary(buffer.data(), buffer.size()) == binary("123456789ABCDEF1"));
    }
}
