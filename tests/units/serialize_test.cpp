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
 * \brief Test of serialize function.
 */
#include "msgpack_light/serialize.h"

#include <catch2/catch_test_macros.hpp>

#include "msgpack_light/binary.h"
#include "msgpack_light/memory_output_stream.h"

TEST_CASE("msgpack_light::serialize_to") {
    using msgpack_light::binary;
    using msgpack_light::memory_output_stream;
    using msgpack_light::serialize_to;

    SECTION("serialize data") {
        memory_output_stream stream;

        serialize_to(stream, false);

        CHECK(stream.as_binary() == binary("C2"));
    }
}

TEST_CASE("msgpack_light::serialize") {
    using msgpack_light::binary;
    using msgpack_light::memory_output_stream;
    using msgpack_light::serialize;

    SECTION("serialize data") {
        const auto data = serialize(false);

        CHECK(data == binary("C2"));
    }
}
