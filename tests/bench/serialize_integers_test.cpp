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
 * \brief Benchmark of serialization of integers.
 */
#include <random>
#include <string>
#include <vector>

#include <celero/Celero.h>
#include <msgpack.hpp>

#include "msgpack_light/binary.h"
#include "msgpack_light/memory_output_stream.h"
#include "msgpack_light/serialize.h"

class serialize_integers_fixture : public celero::TestFixture {
public:
    serialize_integers_fixture() {
        celero::DoNotOptimizeAway(get_data<std::uint32_t>());
        celero::DoNotOptimizeAway(get_data<std::int32_t>());
    }

    template <typename T>
    static const std::vector<T>& get_data() {
        static auto data = create_data<T>();
        return data;
    }

private:
    template <typename T>
    static std::vector<T> create_data() {
        std::vector<T> data;
        constexpr std::size_t size = 1000;
        data.reserve(size);
        std::mt19937 generator;  // NOLINT
        std::uniform_int_distribution<T> distribution;
        for (std::size_t i = 0; i < size; ++i) {
            data.push_back(distribution(generator));
        }
        return data;
    }
};

// NOLINTNEXTLINE
BASELINE_F(serialize_uint32, msgpack_cxx, serialize_integers_fixture, 100, 0) {
    msgpack::sbuffer buffer;
    const auto& data = get_data<std::uint32_t>();
    for (auto value : data) {
        msgpack::pack(buffer, value);
    }
}

// NOLINTNEXTLINE
BENCHMARK_F(
    serialize_uint32, msgpack_light, serialize_integers_fixture, 100, 0) {
    msgpack_light::memory_output_stream stream;
    const auto& data = get_data<std::uint32_t>();
    for (auto value : data) {
        msgpack_light::serialize_to(stream, value);
    }
}

// NOLINTNEXTLINE
BASELINE_F(serialize_int32, msgpack_cxx, serialize_integers_fixture, 100, 0) {
    msgpack::sbuffer buffer;
    const auto& data = get_data<std::int32_t>();
    for (auto value : data) {
        msgpack::pack(buffer, value);
    }
}

// NOLINTNEXTLINE
BENCHMARK_F(
    serialize_int32, msgpack_light, serialize_integers_fixture, 100, 0) {
    msgpack_light::memory_output_stream stream;
    const auto& data = get_data<std::int32_t>();
    for (auto value : data) {
        msgpack_light::serialize_to(stream, value);
    }
}
