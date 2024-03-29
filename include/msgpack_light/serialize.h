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
 * \brief Definition of serialize function.
 */
#pragma once

#include "msgpack_light/binary.h"
#include "msgpack_light/memory_output_stream.h"
#include "msgpack_light/output_stream.h"
#include "msgpack_light/serialization_buffer.h"
#include "msgpack_light/type_support/common.h"  // IWYU pragma: export

namespace msgpack_light {

/*!
 * \brief Serialize data in memory and return the resulting binary data.
 *
 * \tparam T Type of data.
 * \param[out] stream Stream to write serialized data.
 * \param[in] data Data.
 */
template <typename T>
inline void serialize_to(output_stream& stream, const T& data) {
    serialization_buffer buffer(stream);
    buffer.serialize(data);
    buffer.flush();
}

/*!
 * \brief Serialize data to an output stream.
 *
 * \tparam T Type of data to serialize.
 * \param[in] data Data to serialize.
 * \return Serialized binary data.
 */
template <typename T>
[[nodiscard]] inline binary serialize(const T& data) {
    memory_output_stream stream;
    serialize_to(stream, data);
    return stream.as_binary();
}

}  // namespace msgpack_light
