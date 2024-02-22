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
 * \brief Test of basic_binary_buffer class.
 */
#include "msgpack_light/details/basic_binary_buffer.h"

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("msgpack_light::details::basic_binary_buffer") {
    using msgpack_light::details::basic_binary_buffer;

    SECTION("create a buffer") {
        constexpr std::size_t size = 37U;

        const auto buffer = basic_binary_buffer(size);

        CHECK(static_cast<const void*>(buffer.data()) != nullptr);
        CHECK(buffer.size() == size);
    }

    SECTION("create an empty buffer") {
        constexpr std::size_t size = 0U;

        const auto buffer = basic_binary_buffer(size);

        CHECK(buffer.size() == size);
    }

    SECTION("write to a buffer") {
        constexpr std::size_t size = 5U;
        auto buffer = basic_binary_buffer(size);

        for (std::size_t i = 0; i < size; ++i) {
            // If a buffer is correctly created, the following line won't cause
            // errors in sanitizers nor segmentation faults.
            buffer.data()[i] = static_cast<unsigned char>(i);
        }

        for (std::size_t i = 0; i < size; ++i) {
            INFO("i = " << i);
            CHECK(static_cast<std::size_t>(buffer.data()[i]) == i);
        }
    }

    SECTION("change the size") {
        constexpr std::size_t size1 = 5U;
        auto buffer = basic_binary_buffer(size1);
        for (std::size_t i = 0; i < size1; ++i) {
            buffer.data()[i] = static_cast<unsigned char>(i);
        }

        constexpr std::size_t size2 = 7U;
        buffer.resize(size2);

        CHECK(buffer.size() == size2);
        for (std::size_t i = 0; i < size1; ++i) {
            INFO("i = " << i);
            CHECK(static_cast<std::size_t>(buffer.data()[i]) == i);
        }

        SECTION("then write to the extended buffer") {
            for (std::size_t i = size1; i < size2; ++i) {
                // If a buffer is correctly created, the following line won't
                // cause errors in sanitizers nor segmentation faults.
                buffer.data()[i] = static_cast<unsigned char>(i);
            }

            for (std::size_t i = 0; i < size2; ++i) {
                INFO("i = " << i);
                CHECK(static_cast<std::size_t>(buffer.data()[i]) == i);
            }
        }
    }

    SECTION("change the size to zero") {
        constexpr std::size_t size1 = 5U;
        auto buffer = basic_binary_buffer(size1);
        for (std::size_t i = 0; i < size1; ++i) {
            buffer.data()[i] = static_cast<unsigned char>(i);
        }

        constexpr std::size_t size2 = 0;
        buffer.resize(size2);

        CHECK(buffer.size() == size2);
    }

    SECTION("create a copy via copy constructor") {
        constexpr std::size_t size = 5U;
        auto buffer = basic_binary_buffer(size);
        for (std::size_t i = 0; i < size; ++i) {
            buffer.data()[i] = static_cast<unsigned char>(i);
        }

        const basic_binary_buffer copy{buffer};

        CHECK(static_cast<const void*>(buffer.data()) != nullptr);
        CHECK(buffer.size() == size);

        CHECK(static_cast<const void*>(copy.data()) != nullptr);
        CHECK(copy.size() == size);

        CHECK(static_cast<const void*>(copy.data()) !=
            static_cast<const void*>(buffer.data()));

        for (std::size_t i = 0; i < size; ++i) {
            INFO("i = " << i);
            CHECK(static_cast<std::size_t>(copy.data()[i]) == i);
        }
    }

    SECTION("create a copy via copy assignment operator") {
        constexpr std::size_t size = 5U;
        auto buffer = basic_binary_buffer(size);
        for (std::size_t i = 0; i < size; ++i) {
            buffer.data()[i] = static_cast<unsigned char>(i);
        }

        basic_binary_buffer copy{1U};
        copy = buffer;

        CHECK(static_cast<const void*>(buffer.data()) != nullptr);
        CHECK(buffer.size() == size);

        CHECK(static_cast<const void*>(copy.data()) != nullptr);
        CHECK(copy.size() == size);

        CHECK(static_cast<const void*>(copy.data()) !=
            static_cast<const void*>(buffer.data()));

        for (std::size_t i = 0; i < size; ++i) {
            INFO("i = " << i);
            CHECK(static_cast<std::size_t>(copy.data()[i]) == i);
        }
    }

    SECTION("move a buffer via move constructor") {
        constexpr std::size_t size = 5U;
        auto buffer = basic_binary_buffer(size);
        for (std::size_t i = 0; i < size; ++i) {
            buffer.data()[i] = static_cast<unsigned char>(i);
        }

        const basic_binary_buffer moved{std::move(buffer)};

        CHECK(static_cast<const void*>(moved.data()) != nullptr);
        CHECK(moved.size() == size);

        for (std::size_t i = 0; i < size; ++i) {
            INFO("i = " << i);
            CHECK(static_cast<std::size_t>(moved.data()[i]) == i);
        }
    }

    SECTION("move a buffer via move assignment operator") {
        constexpr std::size_t size = 5U;
        auto buffer = basic_binary_buffer(size);
        for (std::size_t i = 0; i < size; ++i) {
            buffer.data()[i] = static_cast<unsigned char>(i);
        }

        basic_binary_buffer moved{1U};
        moved = std::move(buffer);

        CHECK(static_cast<const void*>(moved.data()) != nullptr);
        CHECK(moved.size() == size);

        for (std::size_t i = 0; i < size; ++i) {
            INFO("i = " << i);
            CHECK(static_cast<std::size_t>(moved.data()[i]) == i);
        }
    }

    SECTION("swap two buffers") {
        constexpr std::size_t size1 = 5U;
        auto buffer1 = basic_binary_buffer(size1);
        for (std::size_t i = 0; i < size1; ++i) {
            buffer1.data()[i] = static_cast<unsigned char>(i);
        }

        constexpr std::size_t size2 = 7U;
        constexpr std::size_t offset_for_buffer2 = 10U;
        auto buffer2 = basic_binary_buffer(size2);
        for (std::size_t i = 0; i < size2; ++i) {
            buffer2.data()[i] =
                static_cast<unsigned char>(i + offset_for_buffer2);
        }

        std::swap(buffer1, buffer2);

        REQUIRE(buffer1.size() == size2);
        REQUIRE(buffer2.size() == size1);
        for (std::size_t i = 0; i < size2; ++i) {
            CHECK(static_cast<std::size_t>(buffer1.data()[i]) ==
                i + offset_for_buffer2);
        }
        for (std::size_t i = 0; i < size1; ++i) {
            CHECK(static_cast<std::size_t>(buffer2.data()[i]) == i);
        }
    }
}
