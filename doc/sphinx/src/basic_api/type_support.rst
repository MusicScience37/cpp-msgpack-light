Type Support
=================

Here shows types supported by this library
and the way to add support for other types.

Types Supported by This Library
-------------------------------------

.. list-table:: Currently Supported Types in cpp-msgpack-light Library
    :header-rows: 1
    :widths: 2,1,3

    - - Types in C++
      - Type in MessagePack
      - Header to Include

    - - ``bool``
      - Boolean
      - ``msgpack_light/type_support/common.h``

    - - Integers in 8 bit, 16 bit, 32 bit and, 64 bit (``char``, ``int``, ``uint32_t``, ...)
      - Integer
      - ``msgpack_light/type_support/common.h``

    - - ``float``
      - Float
      - ``msgpack_light/type_support/common.h``

    - - ``double``
      - Float
      - ``msgpack_light/type_support/common.h``

    - - ``std::nullptr_t`` (``nullptr`` object)
      - Nil
      - ``msgpack_light/type_support/common.h``

    - - ``std::string``
      - String
      - ``msgpack_light/type_support/common.h``

    - - ``std::string_view``
      - String
      - ``msgpack_light/type_support/common.h``

    - - ``std::vector``
      - Array
      - ``msgpack_light/type_support/common.h``

    - - ``std::vector<unsigned char>``
      - Binary
      - ``msgpack_light/type_support/common.h``

    - - ``std::array``
      - Array
      - ``msgpack_light/type_support/array.h``

    - - ``std::deque``
      - Array
      - ``msgpack_light/type_support/deque.h``

    - - ``std::forward_list``
      - Array
      - ``msgpack_light/type_support/forward_list.h``

    - - ``std::list``
      - Array
      - ``msgpack_light/type_support/list.h``

    - - ``std::set``
      - Array
      - ``msgpack_light/type_support/set.h``

    - - ``std::multiset``
      - Array
      - ``msgpack_light/type_support/set.h``

    - - ``std::unordered_set``
      - Array
      - ``msgpack_light/type_support/unordered_set.h``

    - - ``std::unordered_multiset``
      - Array
      - ``msgpack_light/type_support/unordered_set.h``

    - - ``std::map``
      - Map
      - ``msgpack_light/type_support/map.h``

    - - ``std::multimap``
      - Map
      - ``msgpack_light/type_support/map.h``

    - - ``std::unordered_map``
      - Map
      - ``msgpack_light/type_support/unordered_map.h``

    - - ``std::unordered_multimap``
      - Map
      - ``msgpack_light/type_support/unordered_map.h``

    - - ``std::pair``
      - Array
      - ``msgpack_light/type_support/pair.h``

    - - ``std::tuple``
      - Array
      - ``msgpack_light/type_support/tuple.h``

    - - ``std::optional``
      - Nil or the type for the value
      - ``msgpack_light/type_support/optional.h``

    - - ``std::chrono::time_point``
      - Extension
      - ``msgpack_light/type_support/chrono.h``

    - - ``std::timespec``
      - Extension
      - ``msgpack_light/type_support/timespec.h``

.. attention::
    Strings are assumed to be encoded in UTF-8.

.. hint::
    Allocators other than ``std::allocator`` can be used.
    For example, ``std::pmr::string`` can be serialized.

Add Support for Other Types
---------------------------------

Users can add support for other types by specializing
``msgpack_light::type_support::serialization_traits``
template or using macros for struct.

Specialization of ``serialization_traits``
```````````````````````````````````````````````

For example, a class ``example::Example`` can be supported as follows:

.. code-block:: cpp

    // Following headers are required for implementation of serialization_traits.
    #include "msgpack_light/serialization_buffer.h"
    #include "msgpack_light/type_support/fwd.h"
    // Other headers may be needed to serialize some existing types.

    namespace msgpack_light::type_support {

    /*!
     * \brief Class to serialize nullptr.
     */
    template <>
    struct serialization_traits<example::Example> {
    public:
        static void serialize(serialization_buffer& buffer, const example::Example& value) {
            // Types already supported can be serialized using "serialize" function.
            buffer.serialize(value.get_parameter());
        }
    };

    }  // namespace msgpack_light::type_support

``serialization_traits`` has a template parameter to be used for SFINAE
as the second template parameter.
For example, classes derived from a class ``example::Base``
can be supported as follows:

.. code-block:: cpp

    // Following headers are required for implementation of serialization_traits.
    #include "msgpack_light/serialization_buffer.h"
    #include "msgpack_light/type_support/fwd.h"
    // Other headers may be needed to serialize some existing types.

    namespace msgpack_light::type_support {

    /*!
     * \brief Class to serialize nullptr.
     */
    template <typename T>
    struct serialization_traits<T, std::enable_if_t<std::is_base_of<example::Base, T>>> {
    public:
        static void serialize(serialization_buffer& buffer, const T& value) {
            // Types already supported can be serialized using "serialize" function.
            buffer.serialize(value.get_parameter());
        }
    };

    }  // namespace msgpack_light::type_support

.. seealso::
    - :cpp:class:`msgpack_light::serialization_buffer`
    - :cpp:func:`msgpack_light::serialization_buffer::serialize`

Macro to Add Support of C++ struct
``````````````````````````````````````

This library provides two macros for support of C++ struct.

- ``MSGPACK_LIGHT_STRUCT_MAP``
- ``MSGPACK_LIGHT_STRUCT_ARRAY``

For example, a struct can be supported as follows:

.. code:: cpp

    namespace example {

    struct Example {
        int param1;
        std::string param2;
    };

    }  // namespace example

    // Add support to serialize into maps
    MSGPACK_LIGHT_STRUCT_MAP(example::Example, param1, param2);
    // or serialize into arrays.
    MSGPACK_LIGHT_STRUCT_ARRAY(example::Example, param1, param2);

Reference
----------------

.. doxygendefine:: MSGPACK_LIGHT_STRUCT_MAP

.. doxygendefine:: MSGPACK_LIGHT_STRUCT_ARRAY

.. doxygenclass:: msgpack_light::serialization_buffer
