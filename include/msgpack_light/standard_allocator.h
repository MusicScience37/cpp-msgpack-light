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
 * \brief Definition of standard_allocator class.
 */
#pragma once

#include <cstddef>
#include <cstdlib>
#include <new>

namespace msgpack_light {

/*!
 * \brief Class of standard allocators to allocate and deallocate memory.
 */
class standard_allocator {
public:
    /*!
     * \brief Allocate memory.
     *
     * \param[in] size Number of bytes to allocate.
     * \return Pointer to the allocated memory.
     */
    [[nodiscard]] void* allocate(  // NOLINT
        std::size_t size, std::size_t /*alignment*/) {
        if (size == 0U) {
            size = 1U;
        }
        void* ptr = std::malloc(size);
        if (ptr == nullptr) {
            throw std::bad_alloc();
        }
        return ptr;
    }

    /*!
     * \brief Reallocate memory.
     *
     * \param[in] ptr Current pointer to the memory.
     * \param[in] new_size New size to allocate.
     * \return Pointer to the allocated memory.
     */
    [[nodiscard]] void* reallocate(  // NOLINT
        void* ptr, std::size_t new_size, std::size_t /*alignment*/) {
        if (new_size == 0U) {
            new_size = 1U;
        }
        void* new_ptr = std::realloc(ptr, new_size);
        if (new_ptr == nullptr) {
            deallocate(ptr);
            throw std::bad_alloc();
        }
        return new_ptr;
    }

    /*!
     * \brief Deallocate memory.
     *
     * \param[in] ptr Pointer to the deallocated memory.
     */
    void deallocate(void* ptr) noexcept {  // NOLINT
        std::free(ptr);
    }
};

}  // namespace msgpack_light
