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
#include <limits>
#include <vector>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "msgpack_light/binary.h"
#include "msgpack_light/monotonic_allocator.h"
#include "msgpack_light/object_data_type.h"
#include "msgpack_light/standard_allocator.h"

TEMPLATE_TEST_CASE("msgpack_light::object", "",
    msgpack_light::standard_allocator, msgpack_light::monotonic_allocator) {
    using msgpack_light::binary;
    using msgpack_light::object;
    using msgpack_light::object_data_type;

    using allocator_type = TestType;
    using object_type = object<allocator_type>;

    SECTION("create an empty object") {
        object_type obj;

        CHECK(obj.type() == object_data_type::nil);
        CHECK_THROWS((void)obj.as_unsigned_integer());
        CHECK_THROWS((void)obj.as_signed_integer());
        CHECK_THROWS((void)obj.as_boolean());
        CHECK_THROWS((void)obj.as_float32());
        CHECK_THROWS((void)obj.as_float64());
        CHECK_THROWS((void)obj.as_string());
        CHECK_THROWS((void)obj.as_binary());
        CHECK_THROWS((void)obj.as_array());

        SECTION("copy") {
            const object_type copy{obj};  // NOLINT

            CHECK(copy.type() == object_data_type::nil);
        }

        SECTION("move") {
            const object_type moved{std::move(obj)};

            CHECK(moved.type() == object_data_type::nil);
        }
    }

    SECTION("create an object of unsigned integer") {
        object_type obj;

        const auto value = GENERATE(static_cast<std::uint64_t>(0),
            static_cast<std::uint64_t>(123),
            std::numeric_limits<std::uint64_t>::max());
        obj.set_unsigned_integer(value);

        CHECK(obj.type() == object_data_type::unsigned_integer);
        CHECK(obj.as_unsigned_integer() == value);
        CHECK_THROWS((void)obj.as_signed_integer());
        CHECK_THROWS((void)obj.as_boolean());
        CHECK_THROWS((void)obj.as_float32());
        CHECK_THROWS((void)obj.as_float64());
        CHECK_THROWS((void)obj.as_string());
        CHECK_THROWS((void)obj.as_binary());
        CHECK_THROWS((void)obj.as_array());

        SECTION("copy") {
            const object_type copy{obj};  // NOLINT

            CHECK(copy.type() == object_data_type::unsigned_integer);
            CHECK(copy.as_unsigned_integer() == value);
        }

        SECTION("move") {
            const object_type moved{std::move(obj)};

            CHECK(moved.type() == object_data_type::unsigned_integer);
            CHECK(moved.as_unsigned_integer() == value);
        }
    }

    SECTION("create an object of signed integer") {
        object_type obj;

        const auto value = GENERATE(std::numeric_limits<std::int64_t>::min(),
            static_cast<std::int64_t>(0), static_cast<std::int64_t>(123),
            std::numeric_limits<std::int64_t>::max());
        obj.set_signed_integer(value);

        CHECK(obj.type() == object_data_type::signed_integer);
        CHECK(obj.as_signed_integer() == value);
        CHECK_THROWS((void)obj.as_unsigned_integer());
        CHECK_THROWS((void)obj.as_boolean());
        CHECK_THROWS((void)obj.as_float32());
        CHECK_THROWS((void)obj.as_float64());
        CHECK_THROWS((void)obj.as_string());
        CHECK_THROWS((void)obj.as_binary());
        CHECK_THROWS((void)obj.as_array());

        SECTION("copy") {
            const object_type copy{obj};  // NOLINT

            CHECK(copy.type() == object_data_type::signed_integer);
            CHECK(copy.as_signed_integer() == value);
        }

        SECTION("move") {
            const object_type moved{std::move(obj)};

            CHECK(moved.type() == object_data_type::signed_integer);
            CHECK(moved.as_signed_integer() == value);
        }
    }

    SECTION("create an object of boolean value") {
        object_type obj;

        const auto value = GENERATE(true, false);
        obj.set_boolean(value);

        CHECK(obj.type() == object_data_type::boolean);
        CHECK(obj.as_boolean() == value);
        CHECK_THROWS((void)obj.as_unsigned_integer());
        CHECK_THROWS((void)obj.as_signed_integer());
        CHECK_THROWS((void)obj.as_float32());
        CHECK_THROWS((void)obj.as_float64());
        CHECK_THROWS((void)obj.as_string());
        CHECK_THROWS((void)obj.as_binary());
        CHECK_THROWS((void)obj.as_array());

        SECTION("copy") {
            const object_type copy{obj};  // NOLINT

            CHECK(copy.type() == object_data_type::boolean);
            CHECK(copy.as_boolean() == value);
        }

        SECTION("move") {
            const object_type moved{std::move(obj)};

            CHECK(moved.type() == object_data_type::boolean);
            CHECK(moved.as_boolean() == value);
        }
    }

    SECTION("create an object of 32-bit floating-point numbers") {
        object_type obj;

        constexpr float value = 1.25F;
        obj.set_float32(value);

        CHECK(obj.type() == object_data_type::float32);
        CHECK(obj.as_float32() == value);
        CHECK_THROWS((void)obj.as_unsigned_integer());
        CHECK_THROWS((void)obj.as_signed_integer());
        CHECK_THROWS((void)obj.as_boolean());
        CHECK_THROWS((void)obj.as_float64());
        CHECK_THROWS((void)obj.as_string());
        CHECK_THROWS((void)obj.as_binary());
        CHECK_THROWS((void)obj.as_array());

        SECTION("copy") {
            const object_type copy{obj};  // NOLINT

            CHECK(copy.type() == object_data_type::float32);
            CHECK(copy.as_float32() == value);
        }

        SECTION("move") {
            const object_type moved{std::move(obj)};

            CHECK(moved.type() == object_data_type::float32);
            CHECK(moved.as_float32() == value);
        }
    }

    SECTION("create an object of 64-bit floating-point numbers") {
        object_type obj;

        constexpr double value = 1.25;
        obj.set_float64(value);

        CHECK(obj.type() == object_data_type::float64);
        CHECK(obj.as_float64() == value);
        CHECK_THROWS((void)obj.as_unsigned_integer());
        CHECK_THROWS((void)obj.as_signed_integer());
        CHECK_THROWS((void)obj.as_boolean());
        CHECK_THROWS((void)obj.as_float32());
        CHECK_THROWS((void)obj.as_string());
        CHECK_THROWS((void)obj.as_binary());
        CHECK_THROWS((void)obj.as_array());

        SECTION("copy") {
            const object_type copy{obj};  // NOLINT

            CHECK(copy.type() == object_data_type::float64);
            CHECK(copy.as_float64() == value);
        }

        SECTION("move") {
            const object_type moved{std::move(obj)};

            CHECK(moved.type() == object_data_type::float64);
            CHECK(moved.as_float64() == value);
        }
    }

    SECTION("create an object of a string") {
        object_type obj;

        const auto value = GENERATE(std::string(), std::string("a"),
            std::string("ab"), std::string("abc"));
        obj.set_string(value);

        CHECK(obj.type() == object_data_type::string);
        CHECK(obj.as_string() == value);
        CHECK_THROWS((void)obj.as_unsigned_integer());
        CHECK_THROWS((void)obj.as_signed_integer());
        CHECK_THROWS((void)obj.as_boolean());
        CHECK_THROWS((void)obj.as_float32());
        CHECK_THROWS((void)obj.as_float64());
        CHECK_THROWS((void)obj.as_binary());
        CHECK_THROWS((void)obj.as_array());

        SECTION("copy") {
            const object_type copy{obj};  // NOLINT

            CHECK(copy.type() == object_data_type::string);
            CHECK(copy.as_string() == value);
        }

        SECTION("move") {
            const object_type moved{std::move(obj)};

            CHECK(moved.type() == object_data_type::string);
            CHECK(moved.as_string() == value);
        }
    }

    SECTION("create an object of a binary") {
        object_type obj;

        const auto value =
            GENERATE(binary(), binary("A1"), binary("A1B2"), binary("A1B2C3"));
        obj.set_binary(value);

        CHECK(obj.type() == object_data_type::binary);
        CHECK(obj.as_binary() == value);
        CHECK_THROWS((void)obj.as_unsigned_integer());
        CHECK_THROWS((void)obj.as_signed_integer());
        CHECK_THROWS((void)obj.as_boolean());
        CHECK_THROWS((void)obj.as_float32());
        CHECK_THROWS((void)obj.as_float64());
        CHECK_THROWS((void)obj.as_string());
        CHECK_THROWS((void)obj.as_array());

        SECTION("copy") {
            const object_type copy{obj};  // NOLINT

            CHECK(copy.type() == object_data_type::binary);
            CHECK(copy.as_binary() == value);
        }

        SECTION("move") {
            const object_type moved{std::move(obj)};

            CHECK(moved.type() == object_data_type::binary);
            CHECK(moved.as_binary() == value);
        }
    }

    SECTION("create an object of an array") {
        object_type obj;

        {
            auto array_ref = obj.set_array(3U);
            array_ref[0].set_unsigned_integer(5U);  // NOLINT
            array_ref[1].set_array(1U);
        }

        {
            CHECK(obj.type() == object_data_type::array);
            const auto array_ref = obj.as_array();
            REQUIRE(array_ref.size() == 3U);
            CHECK(array_ref[0].as_unsigned_integer() == 5U);  // NOLINT
            CHECK(array_ref[1].type() == object_data_type::array);
            REQUIRE(array_ref[1].as_array().size() == 1U);
            CHECK(array_ref[1].as_array()[0].type() == object_data_type::nil);
            CHECK(array_ref[2].type() == object_data_type::nil);
        }
        CHECK_THROWS((void)obj.as_unsigned_integer());
        CHECK_THROWS((void)obj.as_signed_integer());
        CHECK_THROWS((void)obj.as_boolean());
        CHECK_THROWS((void)obj.as_float32());
        CHECK_THROWS((void)obj.as_float64());
        CHECK_THROWS((void)obj.as_string());
        CHECK_THROWS((void)obj.as_binary());

        SECTION("decrease size") {
            auto array_ref = obj.as_array();
            array_ref.resize(1U);
            REQUIRE(array_ref.size() == 1U);
            CHECK(array_ref[0].as_unsigned_integer() == 5U);  // NOLINT
        }

        SECTION("increase size") {
            auto array_ref = obj.as_array();
            array_ref.resize(4U);  // NOLINT
            REQUIRE(array_ref.size() == 4U);
            CHECK(array_ref[0].as_unsigned_integer() == 5U);  // NOLINT
            CHECK(array_ref[1].as_array().size() == 1U);
            CHECK(array_ref[2].type() == object_data_type::nil);
        }

        SECTION("copy") {
            const object_type copy{obj};  // NOLINT

            CHECK(copy.type() == object_data_type::array);
            const auto array_ref = copy.as_array();
            REQUIRE(array_ref.size() == 3U);
            CHECK(array_ref[0].as_unsigned_integer() == 5U);  // NOLINT
            CHECK(array_ref[1].type() == object_data_type::array);
            REQUIRE(array_ref[1].as_array().size() == 1U);
            CHECK(array_ref[1].as_array()[0].type() == object_data_type::nil);
            CHECK(array_ref[2].type() == object_data_type::nil);
        }

        SECTION("move") {
            const object_type moved{std::move(obj)};

            CHECK(moved.type() == object_data_type::array);
            const auto array_ref = moved.as_array();
            REQUIRE(array_ref.size() == 3U);
            CHECK(array_ref[0].as_unsigned_integer() == 5U);  // NOLINT
            CHECK(array_ref[1].type() == object_data_type::array);
            REQUIRE(array_ref[1].as_array().size() == 1U);
            CHECK(array_ref[1].as_array()[0].type() == object_data_type::nil);
            CHECK(array_ref[2].type() == object_data_type::nil);
        }
    }

    SECTION("iterate over an array") {
        object_type obj;

        {
            auto array_ref = obj.set_array(3U);
            array_ref[0].set_signed_integer(1);
            array_ref[1].set_signed_integer(2);
            array_ref[2].set_signed_integer(3);
        }

        SECTION("using const_iterator in const_array_ref") {
            const auto& const_obj = obj;

            std::vector<std::int64_t> values;
            values.reserve(const_obj.as_array().size());
            for (const auto& elem : const_obj.as_array()) {
                REQUIRE_NOTHROW(values.push_back(elem.as_signed_integer()));
            }
            CHECK(values == std::vector<std::int64_t>{1, 2, 3});
        }

        SECTION("using iterator in array_ref") {
            std::vector<std::int64_t> values;
            values.reserve(obj.as_array().size());
            for (const auto& elem : obj.as_array()) {
                REQUIRE_NOTHROW(values.push_back(elem.as_signed_integer()));
            }
            CHECK(values == std::vector<std::int64_t>{1, 2, 3});
        }

        SECTION("using const_iterator in array_ref") {
            std::vector<std::int64_t> values;
            const auto array_ref = obj.as_array();
            values.reserve(obj.as_array().size());
            for (const auto& elem : array_ref) {
                REQUIRE_NOTHROW(values.push_back(elem.as_signed_integer()));
            }
            CHECK(values == std::vector<std::int64_t>{1, 2, 3});
        }
    }

    SECTION("copy constructor") {
        object_type obj;
        {
            auto array_ref = obj.set_array(3U);
            array_ref[0].set_unsigned_integer(5U);  // NOLINT
            array_ref[1].set_array(1U);
        }

        const object_type copy{obj};  // NOLINT

        {
            CHECK(copy.type() == object_data_type::array);
            const auto array_ref = copy.as_array();
            REQUIRE(array_ref.size() == 3U);
            CHECK(array_ref[0].as_unsigned_integer() == 5U);  // NOLINT
            CHECK(array_ref[1].type() == object_data_type::array);
            REQUIRE(array_ref[1].as_array().size() == 1U);
            CHECK(array_ref[1].as_array()[0].type() == object_data_type::nil);
            CHECK(array_ref[2].type() == object_data_type::nil);
        }
    }

    SECTION("copy assignment operator") {
        object_type obj;
        {
            auto array_ref = obj.set_array(3U);
            array_ref[0].set_unsigned_integer(5U);  // NOLINT
            array_ref[1].set_array(1U);
        }

        object_type copy;
        copy = obj;

        {
            CHECK(copy.type() == object_data_type::array);
            const auto array_ref = copy.as_array();
            REQUIRE(array_ref.size() == 3U);
            CHECK(array_ref[0].as_unsigned_integer() == 5U);  // NOLINT
            CHECK(array_ref[1].type() == object_data_type::array);
            REQUIRE(array_ref[1].as_array().size() == 1U);
            CHECK(array_ref[1].as_array()[0].type() == object_data_type::nil);
            CHECK(array_ref[2].type() == object_data_type::nil);
        }
    }

    SECTION("move constructor") {
        object_type obj;
        {
            auto array_ref = obj.set_array(3U);
            array_ref[0].set_unsigned_integer(5U);  // NOLINT
            array_ref[1].set_array(1U);
        }

        const object_type moved{std::move(obj)};  // NOLINT

        {
            CHECK(moved.type() == object_data_type::array);
            const auto array_ref = moved.as_array();
            REQUIRE(array_ref.size() == 3U);
            CHECK(array_ref[0].as_unsigned_integer() == 5U);  // NOLINT
            CHECK(array_ref[1].type() == object_data_type::array);
            REQUIRE(array_ref[1].as_array().size() == 1U);
            CHECK(array_ref[1].as_array()[0].type() == object_data_type::nil);
            CHECK(array_ref[2].type() == object_data_type::nil);
        }
    }

    SECTION("move assignment operator") {
        object_type obj;
        {
            auto array_ref = obj.set_array(3U);
            array_ref[0].set_unsigned_integer(5U);  // NOLINT
            array_ref[1].set_array(1U);
        }

        object_type moved;
        moved = std::move(obj);

        {
            CHECK(moved.type() == object_data_type::array);
            const auto array_ref = moved.as_array();
            REQUIRE(array_ref.size() == 3U);
            CHECK(array_ref[0].as_unsigned_integer() == 5U);  // NOLINT
            CHECK(array_ref[1].type() == object_data_type::array);
            REQUIRE(array_ref[1].as_array().size() == 1U);
            CHECK(array_ref[1].as_array()[0].type() == object_data_type::nil);
            CHECK(array_ref[2].type() == object_data_type::nil);
        }
    }
}
