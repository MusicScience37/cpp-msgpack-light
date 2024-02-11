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
 * \brief Test of memory_output_stream class.
 */
#include "msgpack_light/memory_output_stream.h"

#include <limits>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "msgpack_light/binary.h"

TEST_CASE("msgpack_light::details::calculate_expanded_memory_buffer_size") {
    using msgpack_light::details::calculate_expanded_memory_buffer_size;

    constexpr std::size_t max_size = std::numeric_limits<std::size_t>::max();

    SECTION("calculate size") {
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        CHECK(calculate_expanded_memory_buffer_size(8U, 2U) == 16U);
    }

    SECTION("calculate size with large additional size") {
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        CHECK(calculate_expanded_memory_buffer_size(8U, 16U) == 32U);
    }

    SECTION("calculate size with almost maximum size") {
        CHECK(calculate_expanded_memory_buffer_size(
                  max_size / 2U, max_size / 2U) == max_size - 1U);
    }

    SECTION("calculate size resulting in the maximum size") {
        CHECK(calculate_expanded_memory_buffer_size(
                  max_size / 2U, max_size / 2U + 1U) == max_size);
    }

    SECTION("try to calculate too large size") {
        CHECK_THROWS((void)calculate_expanded_memory_buffer_size(
            max_size / 2U, max_size / 2U + 2U));
    }
}

TEST_CASE("msgpack_light::memory_output_stream") {
    using msgpack_light::binary;
    using msgpack_light::memory_output_stream;

    SECTION("initialize") {
        memory_output_stream stream;

        CHECK(
            // NOLINTNEXTLINE
            std::vector<unsigned char>(stream.data(),
                stream.data() + stream.size()) == std::vector<unsigned char>());
    }

    SECTION("write data") {
        memory_output_stream stream;

        const auto written_data = binary("010203");
        stream.write(written_data.data(), written_data.size());

        CHECK(stream.as_binary() == written_data);
    }

    SECTION("write data twice") {
        memory_output_stream stream;

        const auto written_data1 = binary("010203");
        stream.write(written_data1.data(), written_data1.size());
        const auto written_data2 = binary("0405");
        stream.write(written_data2.data(), written_data2.size());

        CHECK(stream.as_binary() == written_data1 + written_data2);
    }

    SECTION("write large data") {
        memory_output_stream stream;

        const auto written_data = binary(
            std::vector<unsigned char>(1000, static_cast<unsigned char>(1)));
        stream.write(written_data.data(), written_data.size());

        CHECK(stream.as_binary() == written_data);
    }
}
