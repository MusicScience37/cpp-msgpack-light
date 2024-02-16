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
 * \brief Test to serialize struct.
 */

/*
 * This source is automatically generated from struct_test.cpp.jinja.
 * Do not edit this file directly.
 */

#include <catch2/catch_test_macros.hpp>

#include "example_struct.h"
#include "msgpack_light/binary.h"
#include "msgpack_light/serialize.h"

TEST_CASE("MSGPACK_LIGHT_STRUCT_MAP") {
    using msgpack_light::binary;
    using msgpack_light::serialize;
    using msgpack_light_test::map_example_struct1;
    using msgpack_light_test::map_example_struct10;
    using msgpack_light_test::map_example_struct2;
    using msgpack_light_test::map_example_struct3;
    using msgpack_light_test::map_example_struct4;
    using msgpack_light_test::map_example_struct5;
    using msgpack_light_test::map_example_struct6;
    using msgpack_light_test::map_example_struct7;
    using msgpack_light_test::map_example_struct8;
    using msgpack_light_test::map_example_struct9;

    SECTION("serialize a struct with 1 parameter into a map") {
        const auto value = map_example_struct1{1};
        const auto expected_binary = binary(
            "81"              // fixmap with 1 pair
            "A6706172616D31"  // "param1"
            "01"              // 1
        );

        CHECK(serialize(value) == expected_binary);
    }

    SECTION("serialize a struct with 2 parameter into a map") {
        const auto value = map_example_struct2{1, 2};
        const auto expected_binary = binary(
            "82"              // fixmap with 2 pair
            "A6706172616D31"  // "param1"
            "01"              // 1
            "A6706172616D32"  // "param2"
            "02"              // 2
        );

        CHECK(serialize(value) == expected_binary);
    }

    SECTION("serialize a struct with 3 parameter into a map") {
        const auto value = map_example_struct3{1, 2, 3};
        const auto expected_binary = binary(
            "83"              // fixmap with 3 pair
            "A6706172616D31"  // "param1"
            "01"              // 1
            "A6706172616D32"  // "param2"
            "02"              // 2
            "A6706172616D33"  // "param3"
            "03"              // 3
        );

        CHECK(serialize(value) == expected_binary);
    }

    SECTION("serialize a struct with 4 parameter into a map") {
        const auto value = map_example_struct4{1, 2, 3, 4};
        const auto expected_binary = binary(
            "84"              // fixmap with 4 pair
            "A6706172616D31"  // "param1"
            "01"              // 1
            "A6706172616D32"  // "param2"
            "02"              // 2
            "A6706172616D33"  // "param3"
            "03"              // 3
            "A6706172616D34"  // "param4"
            "04"              // 4
        );

        CHECK(serialize(value) == expected_binary);
    }

    SECTION("serialize a struct with 5 parameter into a map") {
        const auto value = map_example_struct5{1, 2, 3, 4, 5};
        const auto expected_binary = binary(
            "85"              // fixmap with 5 pair
            "A6706172616D31"  // "param1"
            "01"              // 1
            "A6706172616D32"  // "param2"
            "02"              // 2
            "A6706172616D33"  // "param3"
            "03"              // 3
            "A6706172616D34"  // "param4"
            "04"              // 4
            "A6706172616D35"  // "param5"
            "05"              // 5
        );

        CHECK(serialize(value) == expected_binary);
    }

    SECTION("serialize a struct with 6 parameter into a map") {
        const auto value = map_example_struct6{1, 2, 3, 4, 5, 6};
        const auto expected_binary = binary(
            "86"              // fixmap with 6 pair
            "A6706172616D31"  // "param1"
            "01"              // 1
            "A6706172616D32"  // "param2"
            "02"              // 2
            "A6706172616D33"  // "param3"
            "03"              // 3
            "A6706172616D34"  // "param4"
            "04"              // 4
            "A6706172616D35"  // "param5"
            "05"              // 5
            "A6706172616D36"  // "param6"
            "06"              // 6
        );

        CHECK(serialize(value) == expected_binary);
    }

    SECTION("serialize a struct with 7 parameter into a map") {
        const auto value = map_example_struct7{1, 2, 3, 4, 5, 6, 7};
        const auto expected_binary = binary(
            "87"              // fixmap with 7 pair
            "A6706172616D31"  // "param1"
            "01"              // 1
            "A6706172616D32"  // "param2"
            "02"              // 2
            "A6706172616D33"  // "param3"
            "03"              // 3
            "A6706172616D34"  // "param4"
            "04"              // 4
            "A6706172616D35"  // "param5"
            "05"              // 5
            "A6706172616D36"  // "param6"
            "06"              // 6
            "A6706172616D37"  // "param7"
            "07"              // 7
        );

        CHECK(serialize(value) == expected_binary);
    }

    SECTION("serialize a struct with 8 parameter into a map") {
        const auto value = map_example_struct8{1, 2, 3, 4, 5, 6, 7, 8};
        const auto expected_binary = binary(
            "88"              // fixmap with 8 pair
            "A6706172616D31"  // "param1"
            "01"              // 1
            "A6706172616D32"  // "param2"
            "02"              // 2
            "A6706172616D33"  // "param3"
            "03"              // 3
            "A6706172616D34"  // "param4"
            "04"              // 4
            "A6706172616D35"  // "param5"
            "05"              // 5
            "A6706172616D36"  // "param6"
            "06"              // 6
            "A6706172616D37"  // "param7"
            "07"              // 7
            "A6706172616D38"  // "param8"
            "08"              // 8
        );

        CHECK(serialize(value) == expected_binary);
    }

    SECTION("serialize a struct with 9 parameter into a map") {
        const auto value = map_example_struct9{1, 2, 3, 4, 5, 6, 7, 8, 9};
        const auto expected_binary = binary(
            "89"              // fixmap with 9 pair
            "A6706172616D31"  // "param1"
            "01"              // 1
            "A6706172616D32"  // "param2"
            "02"              // 2
            "A6706172616D33"  // "param3"
            "03"              // 3
            "A6706172616D34"  // "param4"
            "04"              // 4
            "A6706172616D35"  // "param5"
            "05"              // 5
            "A6706172616D36"  // "param6"
            "06"              // 6
            "A6706172616D37"  // "param7"
            "07"              // 7
            "A6706172616D38"  // "param8"
            "08"              // 8
            "A6706172616D39"  // "param9"
            "09"              // 9
        );

        CHECK(serialize(value) == expected_binary);
    }

    SECTION("serialize a struct with 10 parameter into a map") {
        const auto value = map_example_struct10{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        const auto expected_binary = binary(
            "8A"                // fixmap with 10 pair
            "A6706172616D31"    // "param1"
            "01"                // 1
            "A6706172616D32"    // "param2"
            "02"                // 2
            "A6706172616D33"    // "param3"
            "03"                // 3
            "A6706172616D34"    // "param4"
            "04"                // 4
            "A6706172616D35"    // "param5"
            "05"                // 5
            "A6706172616D36"    // "param6"
            "06"                // 6
            "A6706172616D37"    // "param7"
            "07"                // 7
            "A6706172616D38"    // "param8"
            "08"                // 8
            "A6706172616D39"    // "param9"
            "09"                // 9
            "A7706172616D3130"  // "param10"
            "0A"                // 10
        );

        CHECK(serialize(value) == expected_binary);
    }
}

TEST_CASE("MSGPACK_LIGHT_STRUCT_ARRAY") {
    using msgpack_light::binary;
    using msgpack_light::serialize;
    using msgpack_light_test::array_example_struct1;
    using msgpack_light_test::array_example_struct10;
    using msgpack_light_test::array_example_struct2;
    using msgpack_light_test::array_example_struct3;
    using msgpack_light_test::array_example_struct4;
    using msgpack_light_test::array_example_struct5;
    using msgpack_light_test::array_example_struct6;
    using msgpack_light_test::array_example_struct7;
    using msgpack_light_test::array_example_struct8;
    using msgpack_light_test::array_example_struct9;

    SECTION("serialize a struct with 1 parameter into an array") {
        const auto value = array_example_struct1{1};
        const auto expected_binary = binary(
            "91"  // fixarray with 1 pair
            "01"  // 1
        );

        CHECK(serialize(value) == expected_binary);
    }

    SECTION("serialize a struct with 2 parameter into an array") {
        const auto value = array_example_struct2{1, 2};
        const auto expected_binary = binary(
            "92"  // fixarray with 2 pair
            "01"  // 1
            "02"  // 2
        );

        CHECK(serialize(value) == expected_binary);
    }

    SECTION("serialize a struct with 3 parameter into an array") {
        const auto value = array_example_struct3{1, 2, 3};
        const auto expected_binary = binary(
            "93"  // fixarray with 3 pair
            "01"  // 1
            "02"  // 2
            "03"  // 3
        );

        CHECK(serialize(value) == expected_binary);
    }

    SECTION("serialize a struct with 4 parameter into an array") {
        const auto value = array_example_struct4{1, 2, 3, 4};
        const auto expected_binary = binary(
            "94"  // fixarray with 4 pair
            "01"  // 1
            "02"  // 2
            "03"  // 3
            "04"  // 4
        );

        CHECK(serialize(value) == expected_binary);
    }

    SECTION("serialize a struct with 5 parameter into an array") {
        const auto value = array_example_struct5{1, 2, 3, 4, 5};
        const auto expected_binary = binary(
            "95"  // fixarray with 5 pair
            "01"  // 1
            "02"  // 2
            "03"  // 3
            "04"  // 4
            "05"  // 5
        );

        CHECK(serialize(value) == expected_binary);
    }

    SECTION("serialize a struct with 6 parameter into an array") {
        const auto value = array_example_struct6{1, 2, 3, 4, 5, 6};
        const auto expected_binary = binary(
            "96"  // fixarray with 6 pair
            "01"  // 1
            "02"  // 2
            "03"  // 3
            "04"  // 4
            "05"  // 5
            "06"  // 6
        );

        CHECK(serialize(value) == expected_binary);
    }

    SECTION("serialize a struct with 7 parameter into an array") {
        const auto value = array_example_struct7{1, 2, 3, 4, 5, 6, 7};
        const auto expected_binary = binary(
            "97"  // fixarray with 7 pair
            "01"  // 1
            "02"  // 2
            "03"  // 3
            "04"  // 4
            "05"  // 5
            "06"  // 6
            "07"  // 7
        );

        CHECK(serialize(value) == expected_binary);
    }

    SECTION("serialize a struct with 8 parameter into an array") {
        const auto value = array_example_struct8{1, 2, 3, 4, 5, 6, 7, 8};
        const auto expected_binary = binary(
            "98"  // fixarray with 8 pair
            "01"  // 1
            "02"  // 2
            "03"  // 3
            "04"  // 4
            "05"  // 5
            "06"  // 6
            "07"  // 7
            "08"  // 8
        );

        CHECK(serialize(value) == expected_binary);
    }

    SECTION("serialize a struct with 9 parameter into an array") {
        const auto value = array_example_struct9{1, 2, 3, 4, 5, 6, 7, 8, 9};
        const auto expected_binary = binary(
            "99"  // fixarray with 9 pair
            "01"  // 1
            "02"  // 2
            "03"  // 3
            "04"  // 4
            "05"  // 5
            "06"  // 6
            "07"  // 7
            "08"  // 8
            "09"  // 9
        );

        CHECK(serialize(value) == expected_binary);
    }

    SECTION("serialize a struct with 10 parameter into an array") {
        const auto value =
            array_example_struct10{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        const auto expected_binary = binary(
            "9A"  // fixarray with 10 pair
            "01"  // 1
            "02"  // 2
            "03"  // 3
            "04"  // 4
            "05"  // 5
            "06"  // 6
            "07"  // 7
            "08"  // 8
            "09"  // 9
            "0A"  // 10
        );

        CHECK(serialize(value) == expected_binary);
    }
}
