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
 * \brief Test of INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS macro.
 */
#include "msgpack_light/details/count_arguments_macro.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS") {
    SECTION("count") {
        STATIC_REQUIRE(INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS(a) == 1);
        STATIC_REQUIRE(INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS(a, a) == 2);
        STATIC_REQUIRE(INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS(a, a, a) == 3);
        STATIC_REQUIRE(INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS(a, a, a, a) == 4);
        STATIC_REQUIRE(
            INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS(a, a, a, a, a) == 5);
        STATIC_REQUIRE(
            INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS(a, a, a, a, a, a) == 6);
        STATIC_REQUIRE(
            INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS(a, a, a, a, a, a, a) == 7);
        STATIC_REQUIRE(INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS(
                           a, a, a, a, a, a, a, a) == 8);
        STATIC_REQUIRE(INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS(
                           a, a, a, a, a, a, a, a, a) == 9);
        STATIC_REQUIRE(INTERNAL_MSGPACK_LIGHT_COUNT_ARGUMENTS(
                           a, a, a, a, a, a, a, a, a, a) == 10);
    }
}
