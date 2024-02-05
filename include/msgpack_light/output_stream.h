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
 * \brief Definition of output_stream class.
 */
#pragma once

#include <cstddef>

namespace msgpack_light {

/*!
 * \brief Interface of streams to write data.
 */
class output_stream {
public:
    /*!
     * \brief Write data.
     *
     * \param[in] data Pointer to the data.
     * \param[in] size Size of the data.
     */
    virtual void write(const unsigned char* data, std::size_t size) = 0;

    /*!
     * \brief Constructor.
     */
    output_stream() = default;

protected:
    /*!
     * \brief Destructor.
     */
    ~output_stream() = default;

    /*!
     * \brief Copy constructor.
     */
    output_stream(const output_stream&) = default;

    /*!
     * \brief Move constructor.
     */
    output_stream(output_stream&&) = default;

    /*!
     * \brief Copy assignment operator.
     *
     * \return This.
     */
    output_stream& operator=(const output_stream&) = default;

    /*!
     * \brief Move assignment operator.
     *
     * \return This.
     */
    output_stream& operator=(output_stream&&) = default;
};

}  // namespace msgpack_light
