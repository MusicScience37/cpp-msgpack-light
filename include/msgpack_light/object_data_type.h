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
 * \brief Definition of object_type enumeration.
 */
#pragma once

namespace msgpack_light {

/*!
 * \brief Enumeration to specify types of data in objects.
 */
enum class object_data_type {
    //! Nil.
    nil,

    //! Unsigned integer.
    unsigned_integer,

    //! Signed integer.
    signed_integer,

    //! Boolean.
    boolean,

    //! 32-bit floating-point numbers.
    float32,

    //! 64-bit floating-point numbers.
    float64,

    //! String.
    string,

    //! Binary.
    binary,

    //! Array.
    array,

    //! Map.
    map,

    //! Extension.
    extension
};

}  // namespace msgpack_light
