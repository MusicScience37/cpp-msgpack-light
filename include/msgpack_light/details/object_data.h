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
 * \brief Definition of object_data struct.
 */
#pragma once

#include <cstddef>
#include <cstdint>

#include "msgpack_light/object_data_type.h"

namespace msgpack_light::details {

struct object_data;

/*!
 * \brief Struct of data of strings.
 */
struct string_data {
    //! Data.
    char* data;

    //! Size of the data.
    std::size_t size;
};

/*!
 * \brief Struct of data of binaries.
 */
struct binary_data {
    //! Data.
    unsigned char* data;

    //! Size of the data.
    std::size_t size;
};

/*!
 * \brief Struct of data of arrays.
 */
struct array_data {
    //! Data.
    object_data* data;

    //! Size of the data.
    std::size_t size;
};

struct key_value_pair_data;

/*!
 * \brief Struct of data of maps.
 */
struct map_data {
    //! Data.
    key_value_pair_data* data;

    //! Size of the data.
    std::size_t size;
};

/*!
 * \brief Struct of data of extensions.
 */
struct extension_data {
    //! Type.
    std::int8_t type;

    //! Data.
    unsigned char* data;

    //! Size of the data.
    std::size_t size;
};

/*!
 * \brief Struct of data of objects in MessagePack.
 */
struct object_data {
    //! Data.
    union {
        //! Unsigned integer.
        std::uint64_t unsigned_integer_value;

        //! Signed integer.
        std::int64_t signed_integer_value;

        //! Boolean.
        bool bool_value;

        //! 32-bit floating-point numbers.
        float float_value;

        //! 64-bit floating-point numbers.
        double double_value;

        //! String.
        string_data string_value;

        //! Binary.
        binary_data binary_value;

        //! Array.
        array_data array_value;

        //! Map.
        map_data map_value;

        //! Extension.
        extension_data extension_value;
    } data{};

    //! Type of the data.
    object_data_type type{object_data_type::nil};
};

/*!
 * \brief Struct of data of key-value pairs in maps.
 */
struct key_value_pair_data {
    //! Key.
    object_data key{};

    //! Value.
    object_data value{};
};

}  // namespace msgpack_light::details
