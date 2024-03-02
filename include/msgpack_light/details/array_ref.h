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
 * \brief Definition of classes of references to arrays..
 */
#pragma once

#include <algorithm>
#include <cstring>

#include "msgpack_light/details/allocator_wrapper.h"
#include "msgpack_light/details/array_iterator.h"
#include "msgpack_light/details/object_data.h"
#include "msgpack_light/details/object_helper.h"
#include "msgpack_light/details/object_ref_decl.h"  // IWYU pragma: keep
#include "msgpack_light/standard_allocator.h"

namespace msgpack_light {

/*!
 * \brief Class to access constant arrays.
 */
class const_array_ref {
public:
    //! Type to access constant objects.
    using const_object_ref_type = const_object_ref;

    //! Type of iterators.
    using iterator = const_array_iterator;

    //! Type of iterators.
    using const_iterator = const_array_iterator;

    /*!
     * \brief Constructor.
     *
     * \param[in] data Data.
     */
    explicit const_array_ref(const details::array_data& data) : data_(&data) {}

    /*!
     * \brief Get the size.
     *
     * \return Size.
     */
    [[nodiscard]] std::size_t size() const noexcept { return data_->size; }

    /*!
     * \brief Get an object.
     *
     * \param[in] index Index of the object.
     * \return Object.
     */
    [[nodiscard]] const_object_ref_type operator[](
        std::size_t index) const noexcept;

    /*!
     * \brief Get an iterator to the first element.
     *
     * \return Iterator.
     */
    [[nodiscard]] const_array_iterator begin() const noexcept {
        return const_array_iterator{data_->data};
    }

    /*!
     * \brief Get an iterator to the past-the-end element.
     *
     * \return Iterator.
     */
    [[nodiscard]] const_array_iterator end() const noexcept {
        return const_array_iterator{data_->data + data_->size};
    }

    /*!
     * \brief Get an iterator to the first element.
     *
     * \return Iterator.
     */
    [[nodiscard]] const_array_iterator cbegin() const noexcept {
        return const_array_iterator{data_->data};
    }

    /*!
     * \brief Get an iterator to the past-the-end element.
     *
     * \return Iterator.
     */
    [[nodiscard]] const_array_iterator cend() const noexcept {
        return const_array_iterator{data_->data + data_->size};
    }

private:
    //! Data.
    const details::array_data* data_;
};

/*!
 * \brief Class to access non-constant arrays.
 *
 * \tparam Allocator Type of the allocator.
 */
template <typename Allocator = standard_allocator>
class mutable_array_ref {
public:
    //! Type of the allocator.
    using allocator_type = Allocator;

    //! Type to access non-constant objects.
    using mutable_object_ref_type = mutable_object_ref<Allocator>;

    //! Type to access constant objects.
    using const_object_ref_type = const_object_ref;

    //! Type of iterators.
    using iterator = mutable_array_iterator<Allocator>;

    //! Type of iterators.
    using const_iterator = const_array_iterator;

    /*!
     * \brief Constructor.
     *
     * \param[in] data Data.
     * \param[in] allocator Allocator.
     */
    mutable_array_ref(details::array_data& data,
        details::allocator_wrapper<Allocator>& allocator)
        : data_(&data), allocator_(&allocator) {}

    /*!
     * \brief Change the size.
     *
     * \param[in] size Size.
     */
    void resize(std::size_t size) {
        if (size < data_->size) {
            for (std::size_t i = size; i < data_->size; ++i) {
                details::clear_object_data(data_->data[i], *allocator_);
            }
        }
        details::object_data* new_data = allocator_->allocate_object_data(size);
        std::memcpy(new_data, data_->data,
            std::min(data_->size, size) * sizeof(details::object_data));
        allocator_->deallocate_object_data(data_->data);
        data_->data = new_data;
        if (size > data_->size) {
            std::memset(data_->data + data_->size, 0,
                (size - data_->size) * sizeof(details::object_data));
        }
        data_->size = size;
    }

    /*!
     * \brief Get the size.
     *
     * \return Size.
     */
    [[nodiscard]] std::size_t size() const noexcept { return data_->size; }

    /*!
     * \brief Get an object.
     *
     * \param[in] index Index of the object.
     * \return Object.
     */
    [[nodiscard]] mutable_object_ref_type operator[](
        std::size_t index) noexcept;

    /*!
     * \brief Get an object.
     *
     * \param[in] index Index of the object.
     * \return Object.
     */
    [[nodiscard]] const_object_ref_type operator[](
        std::size_t index) const noexcept;

    /*!
     * \brief Get an iterator to the first element.
     *
     * \return Iterator.
     */
    [[nodiscard]] mutable_array_iterator<Allocator> begin() noexcept {
        return mutable_array_iterator<Allocator>{data_->data, allocator_};
    }

    /*!
     * \brief Get an iterator to the past-the-end element.
     *
     * \return Iterator.
     */
    [[nodiscard]] mutable_array_iterator<Allocator> end() noexcept {
        return mutable_array_iterator<Allocator>{
            data_->data + data_->size, allocator_};
    }

    /*!
     * \brief Get an iterator to the first element.
     *
     * \return Iterator.
     */
    [[nodiscard]] const_array_iterator begin() const noexcept {
        return const_array_iterator{data_->data};
    }

    /*!
     * \brief Get an iterator to the past-the-end element.
     *
     * \return Iterator.
     */
    [[nodiscard]] const_array_iterator end() const noexcept {
        return const_array_iterator{data_->data + data_->size};
    }

private:
    //! Data.
    details::array_data* data_;

    //! Allocator.
    details::allocator_wrapper<Allocator>* allocator_;
};

}  // namespace msgpack_light
