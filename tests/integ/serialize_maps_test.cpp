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
 * \brief Test to serialize maps.
 */
#include <cstddef>
#include <map>
#include <string>

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_range.hpp>
#include <msgpack.hpp>

#include "msgpack_light/binary.h"
#include "msgpack_light/serialize.h"
#include "msgpack_light/type_support/map.h"  // IWYU pragma: keep

TEST_CASE("serialize std::map<int, std::string>") {
    using msgpack_light::binary;
    using msgpack_light::serialize;

    SECTION("serialize sample maps") {
        const std::map<int, std::string> value = GENERATE(
            std::map<int, std::string>{}, std::map<int, std::string>{{1, "a"}},
            std::map<int, std::string>{{1, "a"}, {2, "b"}},
            std::map<int, std::string>{{1, "a"}, {2, "b"}, {3, "c"}});

        const binary serialized = serialize(value);

        const msgpack::object_handle deserialized =
            msgpack::unpack(reinterpret_cast<const char*>(serialized.data()),
                serialized.size());

        CHECK(deserialized->as<std::map<int, std::string>>() == value);
    }

    SECTION("serialize maps of different sizes") {
        const std::size_t size = GENERATE(
            range(static_cast<std::size_t>(0), static_cast<std::size_t>(300)));
        INFO("size = " << size);

        std::map<int, std::string> value;
        for (std::size_t i = 0; i < size; ++i) {
            value.try_emplace(static_cast<int>(i), std::to_string(i));
        }

        const binary serialized = serialize(value);

        const msgpack::object_handle deserialized =
            msgpack::unpack(reinterpret_cast<const char*>(serialized.data()),
                serialized.size());

        CHECK(deserialized->as<std::map<int, std::string>>() == value);
    }
}
