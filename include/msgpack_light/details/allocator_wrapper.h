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
#include <utility>

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
     * \brief Allocator object data.
     *
     * \param[in] size Number of elements.
     * \return Pointer to the allocated object data.
     */
    [[nodiscard]] object_data* allocate_object_data(std::size_t size) {
        return static_cast<object_data*>(allocator_.allocate(
            size * sizeof(object_data), alignof(object_data)));
    }

    /*!
     * \brief Reallocate object data.
     *
     * \param[in] ptr Existing pointer to object data.
     * \param[in] new_size New number of elements.
     * \return Pointer to the allocated object data.
     */
    [[nodiscard]] object_data* reallocate_object_data(
        object_data* ptr, std::size_t new_size) {
        return static_cast<object_data*>(allocator_.reallocate(
            ptr, new_size * sizeof(object_data), alignof(object_data)));
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
     * \brief Allocator unsigned char objects.
     *
     * \param[in] size Number of elements.
     * \return Pointer to the allocated unsigned char objects.
     */
    [[nodiscard]] unsigned char* allocate_unsigned_char(std::size_t size) {
        return static_cast<unsigned char*>(allocator_.allocate(size, 1U));
    }

    /*!
     * \brief Deallocate unsigned char objects.
     *
     * \param[in] ptr Pointer to the unsigned char objects.
     */
    void deallocate_unsigned_char(unsigned char* ptr) noexcept {
        allocator_.deallocate(ptr);
    }

    /*!
     * \brief Allocator char objects.
     *
     * \param[in] size Number of elements.
     * \return Pointer to the allocated char objects.
     */
    [[nodiscard]] char* allocate_char(std::size_t size) {
        return static_cast<char*>(allocator_.allocate(size, 1U));
    }

    /*!
     * \brief Deallocate char objects.
     *
     * \param[in] ptr Pointer to the char objects.
     */
    void deallocate_char(char* ptr) noexcept { allocator_.deallocate(ptr); }

private:
    //! Allocator.
    Allocator allocator_;
};

}  // namespace msgpack_light::details
