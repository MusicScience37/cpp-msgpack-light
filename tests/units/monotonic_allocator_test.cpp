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
 * \brief Test of monotonic_allocator class.
 */
#include "msgpack_light/monotonic_allocator.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("msgpack_light::monotonic_allocator") {
    using msgpack_light::monotonic_allocator;

    SECTION("allocate some bytes") {
        monotonic_allocator allocator;

        constexpr std::size_t size1 = 7U;
        constexpr std::size_t alignment1 = 4U;
        void* ptr1 = allocator.allocate(size1, alignment1);
        REQUIRE(ptr1 != nullptr);

        SECTION("allocate more bytes") {
            constexpr std::size_t size2 = 11U;
            constexpr std::size_t alignment2 = 8U;
            void* ptr2 = allocator.allocate(size2, alignment2);
            REQUIRE(ptr2 != nullptr);
            REQUIRE(
                ptr2 >= static_cast<void*>(static_cast<char*>(ptr1) + size1));
        }
    }

    SECTION("allocate zero byte") {
        monotonic_allocator allocator;

        void* ptr = allocator.allocate(0U, 1U);
        REQUIRE(ptr != nullptr);
    }

    SECTION("allocate many buffers") {
        monotonic_allocator allocator;

        constexpr std::size_t size = 100U;
        constexpr std::size_t num_buffers = 100U;
        STATIC_REQUIRE(size <= monotonic_allocator::max_allocation_from_buffer);
        STATIC_REQUIRE(
            size * num_buffers > monotonic_allocator::initial_buffer_size);

        for (std::size_t i = 0; i < num_buffers; ++i) {
            INFO("i = " << i);
            constexpr std::size_t alignment = 4U;
            void* ptr = allocator.allocate(size, alignment);
            REQUIRE(ptr != nullptr);
        }
    }

    SECTION("allocate large buffer") {
        monotonic_allocator allocator;

        constexpr std::size_t size = 1000U;
        STATIC_REQUIRE(size > monotonic_allocator::max_allocation_from_buffer);

        constexpr std::size_t num_buffers = 100U;
        for (std::size_t i = 0; i < num_buffers; ++i) {
            INFO("i = " << i);
            constexpr std::size_t alignment = 4U;
            void* ptr = allocator.allocate(size, alignment);
            REQUIRE(ptr != nullptr);
        }
    }
}
