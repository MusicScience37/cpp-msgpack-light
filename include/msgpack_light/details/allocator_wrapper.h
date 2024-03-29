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
 * \brief Definition of allocator_wrapper class.
 */
#pragma once

#include <cstddef>
#include <utility>  // IWYU pragma: keep

#include "msgpack_light/details/object_data.h"

namespace msgpack_light::details {

/*!
 * \brief Class to wrap allocators.
 *
 * \tparam Allocator Type of the allocator.
 */
template <typename Allocator>
class allocator_wrapper {
public:
    /*!
     * \brief Constructor.
     *
     * \param[in] allocator Allocator.
     */
    explicit allocator_wrapper(Allocator allocator)
        : allocator_(std::move(allocator)) {}

    /*!
     * \brief Allocate object data.
     *
     * \param[in] size Number of elements.
     * \return Pointer to the allocated object data.
     */
    [[nodiscard]] object_data* allocate_object_data(std::size_t size) {
        return static_cast<object_data*>(allocator_.allocate(
            size * sizeof(object_data), alignof(object_data)));
    }

    /*!
     * \brief Deallocate object data.
     *
     * \param[in] ptr Pointer to the object data.
     */
    void deallocate_object_data(object_data* ptr) noexcept {
        allocator_.deallocate(ptr);
    }

    /*!
     * \brief Allocate key-value pair data.
     *
     * \param[in] size Number of pairs.
     * \return Pointer to the allocated key-value pair data.
     */
    [[nodiscard]] key_value_pair_data* allocate_key_value_pair_data(
        std::size_t size) {
        return static_cast<key_value_pair_data*>(allocator_.allocate(
            size * sizeof(key_value_pair_data), alignof(key_value_pair_data)));
    }

    /*!
     * \brief Deallocate key-value pair data.
     *
     * \param[in] ptr Pointer to the key-value pair data.
     */
    void deallocate_key_value_pair_data(key_value_pair_data* ptr) noexcept {
        allocator_.deallocate(ptr);
    }

    /*!
     * \brief Allocate unsigned char instances.
     *
     * \param[in] size Number of elements.
     * \return Pointer to the allocated unsigned char instances.
     */
    [[nodiscard]] unsigned char* allocate_unsigned_char(std::size_t size) {
        return static_cast<unsigned char*>(allocator_.allocate(size, 1U));
    }

    /*!
     * \brief Deallocate unsigned char instances.
     *
     * \param[in] ptr Pointer to the unsigned char instances.
     */
    void deallocate_unsigned_char(unsigned char* ptr) noexcept {
        allocator_.deallocate(ptr);
    }

    /*!
     * \brief Allocate char instances.
     *
     * \param[in] size Number of elements.
     * \return Pointer to the allocated char instances.
     */
    [[nodiscard]] char* allocate_char(std::size_t size) {
        return static_cast<char*>(allocator_.allocate(size, 1U));
    }

    /*!
     * \brief Deallocate char instances.
     *
     * \param[in] ptr Pointer to the char instances.
     */
    void deallocate_char(char* ptr) noexcept { allocator_.deallocate(ptr); }

private:
    //! Allocator.
    Allocator allocator_;
};

}  // namespace msgpack_light::details
