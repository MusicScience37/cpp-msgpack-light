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
 * \brief Test of object class.
 */
#include "msgpack_light/object.h"

#include <cstdint>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "msgpack_light/object_data_type.h"
#include "msgpack_light/standard_allocator.h"

TEST_CASE("msgpack_light::object") {
    using msgpack_light::object;
    using msgpack_light::object_data_type;

    using allocator_type = msgpack_light::standard_allocator;
    using object_type = object<allocator_type>;

    SECTION("create an empty object") {
        object_type obj;

        CHECK(obj.type() == object_data_type::nil);
        CHECK_THROWS((void)obj.as_unsigned_integer());
    }

    SECTION("create an object of unsigned integer") {
        object_type obj;

        const auto value = GENERATE(static_cast<std::uint64_t>(0),
            static_cast<std::uint64_t>(123),
            static_cast<std::uint64_t>(0xFFFFFFFFULL));
        obj.set_unsigned_integer(value);

        CHECK(obj.as_unsigned_integer() == value);
    }
}
