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
 * \brief Definition of serialization_buffer_impl alias.
 */
#pragma once

#ifndef MSGPACK_LIGHT_USE_BUFFER_IN_SERIALIZATION
#if defined(MSGPACK_LIGHT_DOCUMENTATION)
/*!
 * \brief Macro to select use of buffers in serialization.
 */
#define MSGPACK_LIGHT_USE_BUFFER_IN_SERIALIZATION \
<compiler-specific-default-value>
#elif defined(__clang__)
#define MSGPACK_LIGHT_USE_BUFFER_IN_SERIALIZATION 1
#elif defined(__GNUC__) || defined(__GNUG__)
#define MSGPACK_LIGHT_USE_BUFFER_IN_SERIALIZATION 0
#else
#define MSGPACK_LIGHT_USE_BUFFER_IN_SERIALIZATION 1
#endif
#endif

#if MSGPACK_LIGHT_USE_BUFFER_IN_SERIALIZATION

#include "msgpack_light/details/buffered_serialization_buffer_impl.h"

namespace msgpack_light::details {

//! Type of internal implementation of serialization_buffer class.
using serialization_buffer_impl = buffered_serialization_buffer_impl;

}  // namespace msgpack_light::details

#else

#include "msgpack_light/details/non_buffered_serialization_buffer_impl.h"

namespace msgpack_light::details {

//! Type of internal implementation of serialization_buffer class.
using serialization_buffer_impl = non_buffered_serialization_buffer_impl;

}  // namespace msgpack_light::details

#endif
