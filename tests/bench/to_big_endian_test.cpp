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
 * \brief Benchmark of to_big_endian functions.
 */
#include "msgpack_light/details/to_big_endian.h"

#include <array>
#include <cstdint>
#include <random>
#include <vector>

#include <celero/Celero.h>
#include <celero/Utilities.h>

class to_big_endian_fixture : public celero::TestFixture {
public:
    to_big_endian_fixture() {
        celero::DoNotOptimizeAway(get_data<std::uint16_t>());
        celero::DoNotOptimizeAway(get_data<std::uint32_t>());
        celero::DoNotOptimizeAway(get_data<std::uint64_t>());
        celero::DoNotOptimizeAway(get_buffer<2U>());
        celero::DoNotOptimizeAway(get_buffer<4U>());
        celero::DoNotOptimizeAway(get_buffer<8U>());  // NOLINT
    }

    template <typename T>
    static const std::vector<T>& get_data() {
        static auto data = create_data<T>();
        return data;
    }

    template <std::size_t N>
    static std::vector<std::array<unsigned char, N>>& get_buffer() {
        static auto buffer = create_buffer<N>();
        return buffer;
    }

    static constexpr std::size_t size = 1000;

private:
    template <typename T>
    static std::vector<T> create_data() {
        std::vector<T> data;
        data.reserve(size);
        std::mt19937 generator;  // NOLINT
        std::uniform_int_distribution<T> distribution;
        for (std::size_t i = 0; i < size; ++i) {
            data.push_back(distribution(generator));
        }
        return data;
    }

    template <std::size_t N>
    static std::vector<std::array<unsigned char, N>> create_buffer() {
        std::vector<std::array<unsigned char, N>> buffer;
        buffer.resize(size);
        return buffer;
    }
};

// NOLINTNEXTLINE
BASELINE_F(to_big_endian2, default, to_big_endian_fixture, 100, 0) {
    using integer_type = std::uint16_t;
    const auto& data = get_data<integer_type>();
    auto& buffer = get_buffer<sizeof(integer_type)>();
    for (std::size_t i = 0; i < size; ++i) {
        msgpack_light::details::to_big_endian(&data[i], &buffer[i]);
    }
}

// NOLINTNEXTLINE
BASELINE_F(to_big_endian4, default, to_big_endian_fixture, 100, 0) {
    using integer_type = std::uint32_t;
    const auto& data = get_data<integer_type>();
    auto& buffer = get_buffer<sizeof(integer_type)>();
    for (std::size_t i = 0; i < size; ++i) {
        msgpack_light::details::to_big_endian(&data[i], &buffer[i]);
    }
}

// NOLINTNEXTLINE
BASELINE_F(to_big_endian8, default, to_big_endian_fixture, 100, 0) {
    using integer_type = std::uint64_t;
    const auto& data = get_data<integer_type>();
    auto& buffer = get_buffer<sizeof(integer_type)>();
    for (std::size_t i = 0; i < size; ++i) {
        msgpack_light::details::to_big_endian(&data[i], &buffer[i]);
    }
}
