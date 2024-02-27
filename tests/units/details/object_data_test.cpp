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
 * \brief Test of object_data struct.
 */
#include "msgpack_light/details/object_data.h"

#include <type_traits>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("msgpack_light::details::object_data") {
    using msgpack_light::details::object_data;

    SECTION("check of type") {
        STATIC_REQUIRE(std::is_trivially_copyable_v<object_data>);
    }
}
