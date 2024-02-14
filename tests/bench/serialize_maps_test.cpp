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
 * \brief Benchmark of serialization of arrays.
 */
#include <cstddef>
#include <cstdint>
#include <random>
#include <vector>

#include <celero/Celero.h>
#include <msgpack.hpp>

#include "msgpack_light/memory_output_stream.h"
#include "msgpack_light/serialization_buffer.h"
#include "msgpack_light/type_support/common.h"  // IWYU pragma: keep
#include "msgpack_light/type_support/unordered_map.h"

class serialize_maps_fixture : public celero::TestFixture {
public:
    serialize_maps_fixture() { celero::DoNotOptimizeAway(get_data()); }

    static const std::vector<std::unordered_map<int, bool>>& get_data() {
        static auto data = create_data();
        return data;
    }

private:
    static std::vector<std::unordered_map<int, bool>> create_data() {
        std::vector<std::unordered_map<int, bool>> data;
#ifndef NDEBUG
        constexpr std::size_t size = 1;
#else
        constexpr std::size_t size = 100;
#endif
        data.reserve(size);

        std::mt19937 generator;  // NOLINT

        constexpr std::size_t min_size = 0;
        constexpr std::size_t max_size = 300;
        std::uniform_int_distribution<std::size_t> size_distribution{
            min_size, max_size};

        for (std::size_t i = 0; i < size; ++i) {
            const std::size_t size = size_distribution(generator);
            std::unordered_map<int, bool> map;
            map.reserve(size);
            for (std::size_t j = 0; j < size; ++j) {
                map.try_emplace(static_cast<int>(j), j % 2U == 0U);
            }
            data.push_back(std::move(map));
        }
        return data;
    }
};

// NOLINTNEXTLINE
BASELINE_F(serialize_map, msgpack_cxx, serialize_maps_fixture, 100, 0) {
    msgpack::sbuffer buffer;
    const auto& data = get_data();
    for (const auto& value : data) {
        msgpack::pack(buffer, value);
    }
}

// NOLINTNEXTLINE
BENCHMARK_F(serialize_map, msgpack_light, serialize_maps_fixture, 100, 0) {
    msgpack_light::memory_output_stream stream;
    msgpack_light::serialization_buffer buffer(stream);
    const auto& data = get_data();
    for (const auto& value : data) {
        buffer.serialize(value);
    }
}
