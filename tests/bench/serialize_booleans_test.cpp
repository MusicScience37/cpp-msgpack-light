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
 * \brief Benchmark of serialization of boolean values.
 */
#include <cstddef>
#include <vector>

#include <celero/Celero.h>
#include <msgpack.hpp>

#include "msgpack_light/memory_output_stream.h"
#include "msgpack_light/serialization_buffer.h"
#include "msgpack_light/type_support/common.h"  // IWYU pragma: keep

class serialize_booleans_fixture : public celero::TestFixture {
public:
    serialize_booleans_fixture() { celero::DoNotOptimizeAway(get_data()); }

    static const std::vector<bool>& get_data() {
        static auto data = create_data();
        return data;
    }

private:
    static std::vector<bool> create_data() {
        std::vector<bool> data;
        constexpr std::size_t size = 1000;
        data.reserve(size);
        for (std::size_t i = 0; i < size; ++i) {
            data.push_back(i % 2 == 0);
        }
        return data;
    }
};

// NOLINTNEXTLINE
BASELINE_F(
    serialize_booleans, msgpack_cxx, serialize_booleans_fixture, 100, 0) {
    msgpack::sbuffer buffer;
    const auto& data = get_data();
    for (bool value : data) {
        msgpack::pack(buffer, value);
    }
}

// NOLINTNEXTLINE
BENCHMARK_F(
    serialize_booleans, msgpack_light, serialize_booleans_fixture, 100, 0) {
    msgpack_light::memory_output_stream stream;
    msgpack_light::serialization_buffer buffer(stream);
    const auto& data = get_data();
    for (bool value : data) {
        buffer.serialize(value);
    }
}
