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
 * \brief Test of binary class.
 */
#include "msgpack_light/binary.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("msgpack_light::binary") {
    using msgpack_light::binary;

    SECTION("create a buffer") {
        constexpr std::size_t size = 37U;

        const auto buffer = binary(size);

        CHECK(static_cast<const void*>(buffer.data()) != nullptr);
        CHECK(buffer.size() == size);
        CHECK(buffer.capacity() >= buffer.size());
    }

    SECTION("create an empty buffer") {
        constexpr std::size_t size = 0U;

        const auto buffer = binary(size);

        CHECK(buffer.size() == size);
        CHECK(buffer.capacity() >= buffer.size());
    }

    SECTION("create a buffer from the pointer and the size of data") {
        const auto original_data =
            std::vector<unsigned char>{static_cast<unsigned char>(1U),
                static_cast<unsigned char>(2U), static_cast<unsigned char>(3U)};

        const auto buffer = binary(original_data.data(), original_data.size());

        CHECK(buffer.size() == 3U);
        CHECK(buffer.capacity() >= buffer.size());
        CHECK(static_cast<int>(buffer[0]) == 1);
        CHECK(static_cast<int>(buffer[1]) == 2);
        CHECK(static_cast<int>(buffer[2]) == 3);
    }

    SECTION("create a buffer from std::vector") {
        const auto original_data =
            std::vector<unsigned char>{static_cast<unsigned char>(1U),
                static_cast<unsigned char>(2U), static_cast<unsigned char>(3U)};

        const auto buffer = binary(original_data);

        CHECK(buffer.size() == 3U);
        CHECK(buffer.capacity() >= buffer.size());
        CHECK(static_cast<int>(buffer[0]) == 1);
        CHECK(static_cast<int>(buffer[1]) == 2);
        CHECK(static_cast<int>(buffer[2]) == 3);
    }

    SECTION("create a buffer using initializer list") {
        const auto buffer = binary{static_cast<unsigned char>(1U),
            static_cast<unsigned char>(2U), static_cast<unsigned char>(3U)};

        CHECK(buffer.size() == 3U);
        CHECK(buffer.capacity() >= buffer.size());
        CHECK(static_cast<int>(buffer[0]) == 1);
        CHECK(static_cast<int>(buffer[1]) == 2);
        CHECK(static_cast<int>(buffer[2]) == 3);
    }

    SECTION("create a buffer from a hex expression") {
        const auto buffer = binary("A1B2C3D4");

        CHECK(buffer.size() == 4U);
        CHECK(buffer.capacity() >= buffer.size());
        CHECK(static_cast<int>(buffer[0]) == 0xA1);
        CHECK(static_cast<int>(buffer[1]) == 0xB2);
        CHECK(static_cast<int>(buffer[2]) == 0xC3);
        CHECK(static_cast<int>(buffer[3]) == 0xD4);
    }

    SECTION("create a buffer from an empty hex expression") {
        const auto buffer = binary("");

        CHECK(buffer.size() == 0U);
        CHECK(buffer.capacity() >= buffer.size());
    }

    SECTION(
        "throw exception when trying to create a buffer from an invalid hex "
        "expression") {
        CHECK_THROWS(binary("0!"));
        CHECK_THROWS(binary("G0"));
        CHECK_THROWS(binary("0"));
    }

    SECTION("write to a buffer") {
        constexpr std::size_t size = 5U;
        auto buffer = binary(size);

        for (std::size_t i = 0; i < size; ++i) {
            // If a buffer is correctly created, the following line won't cause
            // errors in sanitizers nor segmentation faults.
            buffer[i] = static_cast<unsigned char>(i);
        }

        for (std::size_t i = 0; i < size; ++i) {
            INFO("i = " << i);
            CHECK(static_cast<std::size_t>(buffer[i]) == i);
        }
    }

    SECTION("change the size") {
        constexpr std::size_t size1 = 5U;
        auto buffer = binary(size1);
        for (std::size_t i = 0; i < size1; ++i) {
            buffer[i] = static_cast<unsigned char>(i);
        }

        constexpr std::size_t size2 = 7U;
        buffer.resize(size2);

        CHECK(buffer.size() == size2);
        CHECK(buffer.capacity() >= buffer.size());
        for (std::size_t i = 0; i < size1; ++i) {
            INFO("i = " << i);
            CHECK(static_cast<std::size_t>(buffer[i]) == i);
        }

        SECTION("then write to the extended buffer") {
            for (std::size_t i = size1; i < size2; ++i) {
                // If a buffer is correctly created, the following line won't
                // cause errors in sanitizers nor segmentation faults.
                buffer[i] = static_cast<unsigned char>(i);
            }

            for (std::size_t i = 0; i < size2; ++i) {
                INFO("i = " << i);
                CHECK(static_cast<std::size_t>(buffer[i]) == i);
            }
        }
    }

    SECTION("change the size to zero") {
        constexpr std::size_t size1 = 5U;
        auto buffer = binary(size1);
        for (std::size_t i = 0; i < size1; ++i) {
            buffer[i] = static_cast<unsigned char>(i);
        }

        constexpr std::size_t size2 = 0;
        buffer.resize(size2);

        CHECK(buffer.size() == size2);
        CHECK(buffer.capacity() >= buffer.size());
    }

    SECTION("compare buffers") {
        CHECK(binary("010203") == binary("010203"));
        CHECK(binary("010203") != binary("0102"));
        CHECK(binary("010203") != binary("01020304"));
        CHECK(binary("010203") != binary("010204"));
    }

    SECTION("append data to the end using append function") {
        auto buffer = binary("010203");

        const auto appended_data = binary("040506");
        buffer.append(appended_data.data(), appended_data.size());

        CHECK(buffer == binary("010203040506"));
    }

    SECTION("append data to the end using operator+=") {
        auto buffer = binary("010203");

        const auto appended_data = binary("040506");
        buffer += appended_data;

        CHECK(buffer == binary("010203040506"));
    }

    SECTION("create a copy via copy constructor") {
        constexpr std::size_t size = 5U;
        auto buffer = binary(size);
        for (std::size_t i = 0; i < size; ++i) {
            buffer[i] = static_cast<unsigned char>(i);
        }

        const binary copy{buffer};

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
        auto buffer = binary(size);
        for (std::size_t i = 0; i < size; ++i) {
            buffer[i] = static_cast<unsigned char>(i);
        }

        binary copy{1U};
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
        auto buffer = binary(size);
        for (std::size_t i = 0; i < size; ++i) {
            buffer[i] = static_cast<unsigned char>(i);
        }

        const binary moved{std::move(buffer)};

        CHECK(static_cast<const void*>(moved.data()) != nullptr);
        CHECK(moved.size() == size);

        for (std::size_t i = 0; i < size; ++i) {
            INFO("i = " << i);
            CHECK(static_cast<std::size_t>(moved.data()[i]) == i);
        }
    }

    SECTION("move a buffer via move assignment operator") {
        constexpr std::size_t size = 5U;
        auto buffer = binary(size);
        for (std::size_t i = 0; i < size; ++i) {
            buffer[i] = static_cast<unsigned char>(i);
        }

        binary moved{1U};
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
        auto buffer1 = binary(size1);
        for (std::size_t i = 0; i < size1; ++i) {
            buffer1.data()[i] = static_cast<unsigned char>(i);
        }

        constexpr std::size_t size2 = 7U;
        constexpr std::size_t offset_for_buffer2 = 10U;
        auto buffer2 = binary(size2);
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
