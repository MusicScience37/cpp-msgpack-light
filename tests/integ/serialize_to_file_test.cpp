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
 * \brief Test of serialization to files.
 */
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <msgpack.hpp>

#include "msgpack_light/file_output_stream.h"
#include "msgpack_light/serialize.h"

TEST_CASE("serialize to files") {
    using msgpack_light::file_output_stream;
    using msgpack_light::serialize_to;

    SECTION("serialize") {
        const std::string file_path = "test_integ_serialize_to_file_test.data";
        const std::string value = "abc";

        {
            file_output_stream stream(file_path);

            serialize_to(stream, value);
        }

        {
            std::ifstream stream(file_path, std::ios::binary);
            REQUIRE(static_cast<bool>(stream));
            const auto serialized =
                std::vector<char>(std::istreambuf_iterator<char>(stream),
                    std::istreambuf_iterator<char>());

            const msgpack::object_handle deserialized =
                msgpack::unpack(serialized.data(), serialized.size());

            CHECK(deserialized->as<std::string>() == value);
        }
    }

    SECTION("try to open a file at invalid path") {
        // Current implementation does not create parent directories
        // automatically.
        const std::string file_path = "test/invalid/file/path.data";

        CHECK_THROWS((void)file_output_stream(file_path));
    }
}
