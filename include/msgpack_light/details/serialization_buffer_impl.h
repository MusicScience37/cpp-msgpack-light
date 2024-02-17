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

#include "msgpack_light/details/buffered_serialization_buffer_impl.h"

namespace msgpack_light::details {

//! Type of internal implementation of serialization_buffer class.
using serialization_buffer_impl = buffered_serialization_buffer_impl;

}  // namespace msgpack_light::details
