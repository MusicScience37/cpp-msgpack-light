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
 * \brief Benchmark of serialization of strings.
 */
#include <random>
#include <string>
#include <vector>

#include <celero/Celero.h>
#include <msgpack.hpp>

#include "msgpack_light/binary.h"
#include "msgpack_light/memory_output_stream.h"
#include "msgpack_light/serialize.h"

class serialize_strings_fixture : public celero::TestFixture {
public:
    serialize_strings_fixture() { celero::DoNotOptimizeAway(get_data()); }

    static const std::vector<std::string>& get_data() {
        static auto data = create_data();
        return data;
    }

private:
    static std::vector<std::string> create_data() {
        std::vector<std::string> data;
        constexpr std::size_t size = 1000;
        data.reserve(size);
        std::mt19937 generator;  // NOLINT
        constexpr std::size_t min_size = 0;
        constexpr std::size_t max_size = 1000;
        std::uniform_int_distribution<std::size_t> distribution{
            min_size, max_size};
        for (std::size_t i = 0; i < size; ++i) {
            data.emplace_back(distribution(generator), 'A');
        }
        return data;
    }
};

// NOLINTNEXTLINE
BASELINE_F(serialize_str, msgpack_cxx, serialize_strings_fixture, 100, 0) {
    msgpack::sbuffer buffer;
    const auto& data = get_data();
    for (const auto& value : data) {
        msgpack::pack(buffer, value);
    }
}

// NOLINTNEXTLINE
BENCHMARK_F(serialize_str, msgpack_light, serialize_strings_fixture, 100, 0) {
    msgpack_light::memory_output_stream stream;
    msgpack_light::serialization_buffer buffer(stream);
    const auto& data = get_data();
    for (const auto& value : data) {
        buffer.serialize(value);
    }
}
