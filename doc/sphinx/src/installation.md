# Installation

## Simply Including Headers

This library is header-only,
so you can simply include headers
setting `include` directory to an include directory of compilers.
This library depends only on C++ standard library,
so no further installation is required.

## Via vcpkg

This library can be installed via vcpkg using following configurations:

- Add a vcpkg registry
  [`https://gitlab.com/MusicScience37Projects/vcpkg-registry`](https://gitlab.com/MusicScience37Projects/vcpkg-registry)
  in `vcpkg-configuration.json`.

  Example:

  ```json
  {
    "$schema": "https://raw.githubusercontent.com/microsoft/vcpkg-tool/main/docs/vcpkg-configuration.schema.json",
    "default-registry": {
      "kind": "git",
      "repository": "https://github.com/Microsoft/vcpkg",
      "baseline": "7aeffc91033ad35cc4e2c152f213a866ec6c11ac"
    },
    "registries": [
      {
        "kind": "git",
        "repository": "https://gitlab.com/MusicScience37Projects/vcpkg-registry",
        "baseline": "bd7d4ded3e5312cc66f16bc729317ec64fa4e613",
        "packages": ["cpp-msgpack-light"]
      }
    ]
  }
  ```

- Add `cpp-hash-tables` in `vcpkg.json`

  Example:

  ```json
  {
    "$schema": "https://raw.githubusercontent.com/microsoft/vcpkg-tool/main/docs/vcpkg.schema.json",
    "dependencies": ["cpp-msgpack-light"]
  }
  ```
