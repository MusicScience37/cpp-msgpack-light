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
 * \brief Definition of classes of references to extension values.
 */
#pragma once

#include <cstdint>

#include "msgpack_light/binary.h"
#include "msgpack_light/details/object_data.h"

namespace msgpack_light {

/*!
 * \brief Class to access constant extension value.
 *
 * \note Instances of this class can be created from
 * msgpack_light::object, msgpack_light::const_object_ref,
 * msgpack_light::mutable_object_ref classes.
 *
 * \warning This class only holds pointers to data in msgpack_light::object
 * class, do not call functions in this class without msgpack_light::object
 * instances holding the data.
 */
class const_extension_ref {
public:
    /*!
     * \brief Constructor.
     *
     * \param[in] data Data.
     */
    explicit const_extension_ref(const details::extension_data& data)
        : data_(&data) {}

    /*!
     * \brief Get the type.
     *
     * \return Type.
     */
    [[nodiscard]] std::int8_t type() const { return data_->type; }

    /*!
     * \brief Get the data of the value.
     *
     * \return Data.
     */
    [[nodiscard]] binary_view data() const noexcept {
        return binary_view(data_->data, data_->size);
    }

private:
    //! Data.
    const details::extension_data* data_;
};

}  // namespace msgpack_light
