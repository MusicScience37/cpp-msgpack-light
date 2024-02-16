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
 * \brief Test to serialize more general structs.
 */
#include <cstdint>
#include <string>

#include <catch2/catch_test_macros.hpp>
#include <msgpack.hpp>

#include "example_struct_general.h"
#include "msgpack_light/binary.h"
#include "msgpack_light/serialize.h"

TEST_CASE("serialize more general struct") {
    using msgpack_light::binary;
    using msgpack_light::serialize;
    using msgpack_light_test::array_example_struct;
    using msgpack_light_test::map_example_struct;

    SECTION("serialize") {
        const auto value = array_example_struct{
            "abc", map_example_struct{-12345, "def", false}};

        const binary serialized = serialize(value);

        const msgpack::object_handle deserialized = msgpack::unpack(
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            reinterpret_cast<const char*>(serialized.data()),
            serialized.size());

        REQUIRE(deserialized->type == msgpack::type::ARRAY);
        REQUIRE(deserialized->via.array.size == 2U);
        CHECK(deserialized->via.array.ptr[0].as<std::string>() == "abc");
        REQUIRE(deserialized->via.array.ptr[1].type == msgpack::type::MAP);
        REQUIRE(deserialized->via.array.ptr[1].via.map.size == 3U);
        CHECK(deserialized->via.array.ptr[1]
                  .via.map.ptr[0]
                  .key.as<std::string>() == "id");
        CHECK(deserialized->via.array.ptr[1]
                  .via.map.ptr[0]
                  .val.as<std::int64_t>() == -12345);
        CHECK(deserialized->via.array.ptr[1]
                  .via.map.ptr[1]
                  .key.as<std::string>() == "name");
        CHECK(deserialized->via.array.ptr[1]
                  .via.map.ptr[1]
                  .val.as<std::string>() == "def");
        CHECK(deserialized->via.array.ptr[1]
                  .via.map.ptr[2]
                  .key.as<std::string>() == "flag");
        CHECK(deserialized->via.array.ptr[1].via.map.ptr[2].val.as<bool>() ==
            false);
    }
}
