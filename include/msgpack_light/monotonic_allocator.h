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
 * \brief Definition of monotonic_allocator class.
 */
#pragma once

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <memory>
#include <new>
#include <utility>

namespace msgpack_light {

/*!
 * \brief Class of an allocator which releases memory only when the allocator is
 * destroyed.
 */
class monotonic_allocator {
public:
    //! Size of the initial buffer.
    static constexpr std::size_t initial_buffer_size = 1024U;

    //! Maximum size of memory allocated from buffer.
    static constexpr std::size_t max_allocation_from_buffer = 512U;

    /*!
     * \brief Constructor.
     */
    monotonic_allocator() { prepare_buffer(); }

    /*!
     * \brief Copy constructor.
     *
     * \note This simply initialize this object without any copy.
     */
    monotonic_allocator(const monotonic_allocator& /*other*/) noexcept
        : monotonic_allocator() {}

    /*!
     * \brief Move constructor.
     *
     * \param[in,out] other Object to move from.
     */
    monotonic_allocator(monotonic_allocator&& other) noexcept
        : current_buffer_size_(other.current_buffer_size_),
          current_buffer_(std::exchange(other.current_buffer_, nullptr)),
          next_allocation_point_(
              std::exchange(other.next_allocation_point_, nullptr)),
          remaining_buffer_(other.remaining_buffer_),
          last_direct_allocation_(
              std::exchange(other.last_direct_allocation_, nullptr)) {}

    /*!
     * \brief Copy assignment operator.
     *
     * \note This does nothing.
     *
     * \return This object after copy.
     */
    monotonic_allocator& operator=(  // NOLINT
        const monotonic_allocator& /*other*/) noexcept {
        return *this;
    }

    /*!
     * \brief Move assignment operator.
     *
     * \param[in,out] other Object to move from.
     * \return This object after move.
     */
    monotonic_allocator& operator=(monotonic_allocator&& other) noexcept {
        swap(other);
        return *this;
    }

    /*!
     * \brief Swap with another object.
     *
     * \param[out] other Another object.
     */
    void swap(monotonic_allocator& other) noexcept {
        std::swap(current_buffer_size_, other.current_buffer_size_);
        std::swap(current_buffer_, other.current_buffer_);
        std::swap(next_allocation_point_, other.next_allocation_point_);
        std::swap(remaining_buffer_, other.remaining_buffer_);
        std::swap(last_direct_allocation_, other.last_direct_allocation_);
    }

    /*!
     * \brief Destructor.
     */
    ~monotonic_allocator() {
        while (current_buffer_ != nullptr) {
            void** head = static_cast<void**>(current_buffer_);
            void* prev_buffer = *head;
            std::free(current_buffer_);
            current_buffer_ = prev_buffer;
        }
        while (last_direct_allocation_ != nullptr) {
            void** head = static_cast<void**>(last_direct_allocation_);
            void* prev_buffer = *head;
            std::free(last_direct_allocation_);
            last_direct_allocation_ = prev_buffer;
        }
    }

    /*!
     * \brief Allocate memory.
     *
     * \param[in] size Number of bytes to allocate.
     * \param[in] alignment Alignment.
     * \return Pointer to the allocated memory.
     */
    [[nodiscard]] void* allocate(std::size_t size, std::size_t alignment) {
        if (size == 0U) {
            // Any pointer other than nullptr can be returned.
            return next_allocation_point_;
        }
        if (size > max_allocation_from_buffer) {
            return allocate_directly(size, alignment);
        }
        void* result = try_allocate_from_buffer(size, alignment);
        if (result != nullptr) {
            return result;
        }
        change_buffer();
        return try_allocate_from_buffer(size, alignment);
    }

    /*!
     * \brief Deallocate memory.
     *
     * \note This class does nothing in this function.
     *
     * \param[in] ptr Pointer to the deallocated memory.
     */
    void deallocate(void* ptr) noexcept {  // NOLINT
        // No operation in this class.
        (void)ptr;
    }

private:
    /*!
     * \brief Allocate a buffer.
     *
     * \param[in] size Size of the buffer.
     * \return Pointer to the allocated buffer.
     */
    [[nodiscard]] static void* allocate_buffer(std::size_t size) {
        void* ptr = std::malloc(size);
        if (ptr == nullptr) {
            throw std::bad_alloc();
        }
        return ptr;
    }

    /*!
     * \brief Prepare the next buffer.
     */
    void change_buffer() {
        constexpr std::size_t next_buffer_size_rate = 2U;
        constexpr std::size_t max_buffer_size = static_cast<std::size_t>(1)
            << 20U;
        if (current_buffer_size_ < max_buffer_size) {
            current_buffer_size_ *= next_buffer_size_rate;
        }
        prepare_buffer();
    }

    /*!
     * \brief Prepare a buffer.
     *
     * \note Call this function after update of current_buffer_size_ to the next
     * size.
     */
    void prepare_buffer() {
        void* prev_buffer = current_buffer_;
        current_buffer_ = allocate_buffer(current_buffer_size_);
        void** head = static_cast<void**>(current_buffer_);
        *head = prev_buffer;
        next_allocation_point_ = head + 1;
        remaining_buffer_ = current_buffer_size_ - sizeof(void*);
    }

    /*!
     * \brief Try to allocate from the current buffer.
     *
     * \param[in] size Number of bytes to allocate.
     * \param[in] alignment Alignment.
     * \return Pointer to the allocated memory. Null on failure.
     */
    [[nodiscard]] void* try_allocate_from_buffer(
        std::size_t size, std::size_t alignment) {
        if (std::align(alignment, size, next_allocation_point_,
                remaining_buffer_) == nullptr) {
            return nullptr;
        }
        void* result = next_allocation_point_;
        next_allocation_point_ =
            static_cast<char*>(next_allocation_point_) + size;
        remaining_buffer_ -= size;
        return result;
    }

    /*!
     * \brief Allocate memory directly using malloc function.
     *
     * \param[in] size Size.
     * \param[in] alignment Alignment.
     * \return Pointer to the allocated memory.
     */
    [[nodiscard]] void* allocate_directly(
        std::size_t size, std::size_t alignment) {
        void* prev_allocation = last_direct_allocation_;
        std::size_t size_with_header_and_padding_space =
            size + std::max(sizeof(void*), alignment);
        last_direct_allocation_ =
            allocate_buffer(size_with_header_and_padding_space);
        void** header = static_cast<void**>(last_direct_allocation_);
        *header = prev_allocation;
        void* ptr_after_header = header + 1;
        void* result = std::align(alignment, size, ptr_after_header,
            size_with_header_and_padding_space);
        assert(result != nullptr);
        (void)result;  // prevent error in release build
        return result;
    }

    //! Size of the current buffer.
    std::size_t current_buffer_size_{initial_buffer_size};

    //! Current buffer.
    void* current_buffer_{nullptr};

    //! Pointer to the byte from which the next allocation starts.
    void* next_allocation_point_{nullptr};

    //! Remaining size of the current buffer.
    std::size_t remaining_buffer_{initial_buffer_size};

    //! Last pointer to the memory allocated directly using malloc function.
    void* last_direct_allocation_{nullptr};
};

/*!
 * \brief Swap two objects.
 *
 * \param[in,out] object1 Object.
 * \param[in,out] object2 Object.
 */
inline void swap(msgpack_light::monotonic_allocator& object1,
    msgpack_light::monotonic_allocator& object2) noexcept {
    object1.swap(object2);
}

}  // namespace msgpack_light
