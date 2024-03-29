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
 * \brief Test of total_size_of variable.
 */
#include "msgpack_light/details/total_size_of.h"

#include <cstdint>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("msgpack_light::details::total_size_of") {
    using msgpack_light::details::total_size_of;

    SECTION("get sizes") {
        STATIC_REQUIRE(total_size_of<std::uint8_t> == 1U);
        STATIC_REQUIRE(total_size_of<std::uint16_t> == 2U);
        STATIC_REQUIRE(total_size_of<std::uint16_t, std::uint8_t> == 3U);
        STATIC_REQUIRE(
            total_size_of<std::uint16_t, std::uint8_t, std::uint32_t> == 7U);
    }
}
