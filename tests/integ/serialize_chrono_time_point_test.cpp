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
 * \brief Test to serialize std::chrono::time_point instances.
 */
#include <chrono>

#include <catch2/catch_test_macros.hpp>
#include <msgpack.hpp>

#include "msgpack_light/binary.h"
#include "msgpack_light/serialize.h"
#include "msgpack_light/type_support/chrono.h"  // IWYU pragma: keep

TEST_CASE("serialize std::chrono::system_clock::time_point instances") {
    using msgpack_light::binary;
    using msgpack_light::serialize;

    SECTION("serialize current time") {
        const auto value = std::chrono::system_clock::now();

        const binary serialized = serialize(value);

        const msgpack::object_handle deserialized =
            msgpack::unpack(reinterpret_cast<const char*>(serialized.data()),
                serialized.size());

        CHECK(
            value == deserialized->as<std::chrono::system_clock::time_point>());
    }
}
