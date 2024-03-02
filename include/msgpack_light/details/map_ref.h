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
 * \brief Definition of classes of references to maps.
 */
#pragma once

#include <cstddef>

#include "msgpack_light/details/allocator_wrapper.h"
#include "msgpack_light/details/map_iterator.h"
#include "msgpack_light/details/object_data.h"
#include "msgpack_light/details/object_ref_decl.h"  // IWYU pragma: keep
#include "msgpack_light/standard_allocator.h"

namespace msgpack_light {

/*!
 * \brief Class to access constant maps.
 *
 * \note Instances of this class can be created from
 * msgpack_light::object, msgpack_light::const_object_ref,
 * msgpack_light::mutable_object_ref classes.
 *
 * \warning This class only holds pointers to data in msgpack_light::object
 * class, do not call functions in this class without msgpack_light::object
 * instances holding the data.
 */
class const_map_ref {
public:
    //! Type to access constant objects.
    using const_object_ref_type = const_object_ref;

    //! Type of iterators.
    using iterator = const_map_iterator;

    //! Type of iterators.
    using const_iterator = const_map_iterator;

    /*!
     * \brief Constructor.
     *
     * \param[in] data Data.
     */
    explicit const_map_ref(const details::map_data& data) : data_(&data) {}

    /*!
     * \brief Get the size.
     *
     * \return Size.
     */
    [[nodiscard]] std::size_t size() const noexcept { return data_->size; }

    /*!
     * \brief Get a key.
     *
     * \param[in] index Index of the key-value pair.
     * \return Object of the key.
     */
    [[nodiscard]] const_object_ref_type key(std::size_t index) const noexcept;

    /*!
     * \brief Get a value.
     *
     * \param[in] index Index of the key-value pair.
     * \return Object of the value.
     */
    [[nodiscard]] const_object_ref_type value(std::size_t index) const noexcept;

    /*!
     * \brief Get an iterator to the first key-value pair.
     *
     * \return Iterator.
     */
    [[nodiscard]] const_map_iterator begin() const noexcept {
        return const_map_iterator{data_->data};
    }

    /*!
     * \brief Get an iterator to the past-the-end key-value pair.
     *
     * \return Iterator.
     */
    [[nodiscard]] const_map_iterator end() const noexcept {
        return const_map_iterator{data_->data + data_->size};
    }

private:
    //! Data.
    const details::map_data* data_;
};

/*!
 * \brief Class to access non-constant maps.
 *
 * \tparam Allocator Type of the allocator.
 *
 * \note Instances of this class can be created from
 * msgpack_light::object, msgpack_light::mutable_object_ref classes.
 *
 * \warning This class only holds pointers to data in msgpack_light::object
 * class, do not call functions in this class without msgpack_light::object
 * instances holding the data.
 */
template <typename Allocator = standard_allocator>
class mutable_map_ref {
public:
    //! Type of the allocator.
    using allocator_type = Allocator;

    //! Type to access non-constant objects.
    using mutable_object_ref_type = mutable_object_ref<Allocator>;

    //! Type to access constant objects.
    using const_object_ref_type = const_object_ref;

    //! Type of iterators.
    using iterator = mutable_map_iterator<Allocator>;

    //! Type of iterators.
    using const_iterator = const_map_iterator;

    /*!
     * \brief Constructor.
     *
     * \param[in] data Data.
     * \param[in] allocator Allocator.
     */
    mutable_map_ref(details::map_data& data,
        details::allocator_wrapper<Allocator>& allocator)
        : data_(&data), allocator_(&allocator) {}

    /*!
     * \brief Get the size.
     *
     * \return Size.
     */
    [[nodiscard]] std::size_t size() const noexcept { return data_->size; }

    /*!
     * \brief Get a key.
     *
     * \param[in] index Index of the key-value pair.
     * \return Object of the key.
     */
    [[nodiscard]] mutable_object_ref_type key(std::size_t index) noexcept;

    /*!
     * \brief Get a key.
     *
     * \param[in] index Index of the key-value pair.
     * \return Object of the key.
     */
    [[nodiscard]] const_object_ref_type key(std::size_t index) const noexcept;

    /*!
     * \brief Get a value.
     *
     * \param[in] index Index of the key-value pair.
     * \return Object of the value.
     */
    [[nodiscard]] mutable_object_ref_type value(std::size_t index) noexcept;

    /*!
     * \brief Get a value.
     *
     * \param[in] index Index of the key-value pair.
     * \return Object of the value.
     */
    [[nodiscard]] const_object_ref_type value(std::size_t index) const noexcept;

    /*!
     * \brief Get an iterator to the first key-value pair.
     *
     * \return Iterator.
     */
    [[nodiscard]] mutable_map_iterator<Allocator> begin() noexcept {
        return mutable_map_iterator<Allocator>{data_->data, allocator_};
    }

    /*!
     * \brief Get an iterator to the past-the-end key-value pair.
     *
     * \return Iterator.
     */
    [[nodiscard]] mutable_map_iterator<Allocator> end() noexcept {
        return mutable_map_iterator<Allocator>{
            data_->data + data_->size, allocator_};
    }

    /*!
     * \brief Get an iterator to the first key-value pair.
     *
     * \return Iterator.
     */
    [[nodiscard]] const_map_iterator begin() const noexcept {
        return const_map_iterator{data_->data};
    }

    /*!
     * \brief Get an iterator to the past-the-end key-value pair.
     *
     * \return Iterator.
     */
    [[nodiscard]] const_map_iterator end() const noexcept {
        return const_map_iterator{data_->data + data_->size};
    }

private:
    //! Data.
    details::map_data* data_;

    //! Allocator.
    details::allocator_wrapper<Allocator>* allocator_;
};

}  // namespace msgpack_light
