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
 * \brief Test to serialize floating-point numbers.
 */
#include <catch2/catch_message.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <msgpack.hpp>

#include "msgpack_light/binary.h"
#include "msgpack_light/serialize.h"

TEMPLATE_TEST_CASE("serialize floating-point numbers", "", float, double) {
    using msgpack_light::binary;
    using msgpack_light::serialize;
    using float_type = TestType;

    SECTION("serialize") {
        const float value = GENERATE(take(1000,
            random(static_cast<float_type>(-10), static_cast<float_type>(10))));
        INFO("value = " << value);

        const binary serialized = serialize(value);
        INFO("serialized = " << serialized);

        const msgpack::object_handle deserialized = msgpack::unpack(
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            reinterpret_cast<const char*>(serialized.data()),
            serialized.size());

        CHECK(deserialized->as<float_type>() == value);  // NOLINT
    }
}
