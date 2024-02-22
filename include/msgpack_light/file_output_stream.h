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
 * \brief Definition of file_output_stream class.
 */
#pragma once

#include <cstdio>
#include <stdexcept>
#include <string>

#include "msgpack_light/output_stream.h"

namespace msgpack_light {

/*!
 * \brief Class to write to file.
 */
class file_output_stream final : public output_stream {
public:
    /*!
     * \brief Constructor.
     *
     * \param[in] file_path File path.
     */
    explicit file_output_stream(const char* file_path)
        : file_(std::fopen(file_path, "wb")) {
        if (file_ == nullptr) {
            throw std::runtime_error(
                std::string("Failed to open ") + file_path);
        }
    }

    /*!
     * \brief Constructor.
     *
     * \param[in] file_path File path.
     */
    explicit file_output_stream(const std::string& file_path)
        : file_output_stream(file_path.c_str()) {}

    file_output_stream(const file_output_stream&) = delete;
    file_output_stream(file_output_stream&&) = delete;
    file_output_stream& operator=(const file_output_stream&) = delete;
    file_output_stream& operator=(file_output_stream&&) = delete;

    /*!
     * \brief Destructor.
     */
    ~file_output_stream() { (void)std::fclose(file_); }

    /*!
     * \brief Write data.
     *
     * \param[in] data Pointer to the data.
     * \param[in] size Size of the data.
     */
    void write(const unsigned char* data, std::size_t size) override {
        if (std::fwrite(data, 1U, size, file_) != size) {
            throw std::runtime_error("Failed to write data to a file.");
        }
    }

private:
    //! File descriptor.
    std::FILE* file_;
};

}  // namespace msgpack_light
