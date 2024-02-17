Serialization
=================

Here shows functions and classes to serialize data.

Contents
--------------

- Functions to serialize data

  - :cpp:func:`msgpack_light::serialize`

    - Serialize data in memory and return the resulting binary data.

  - :cpp:func:`msgpack_light::serialize_to`

    - Serialize data to an output stream implementing
      :cpp:class:`msgpack_light::output_stream` interface.

- Output streams

  - :cpp:class:`msgpack_light::output_stream`

    - Interface of output streams used in
      :cpp:func:`msgpack_light::serialize_to` function and
      :cpp:class:`msgpack_light::serialization_buffer` class.
      Write a class implementing this interface
      if a stream not implemented in this library is needed.

  - :cpp:class:`msgpack_light::memory_output_stream`

    - An implementation of :cpp:class:`msgpack_light::output_stream`
      which writes data to buffers in memory.

Reference
----------------

.. doxygenfunction:: msgpack_light::serialize

.. doxygenfunction:: msgpack_light::serialize_to

.. doxygenclass:: msgpack_light::output_stream

.. doxygenclass:: msgpack_light::memory_output_stream
