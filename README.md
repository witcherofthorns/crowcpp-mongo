# Crow + Mongodb
This repository is just an example of how Crowcpp and Mongodb work, you can use this template to build your web backend

## Used

#### Conan
- asio
- mongo-cxx-driver
- nlohmann_json

#### Submodules
- crowcpp

## Building

Create build folder
```bash
mkdir build && cd build
```
Install deps from Conan
```bash
conan install .. --output-folder=conan --build=missing
```
CMake building & compilation (Clnag), used conan toolchain file
```bash
CC=/usr/bin/clang CXX=/usr/bin/clang++ cmake -DCMAKE_BUILD_TYPE=Release .. -G 
"Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=./conan/conan_toolchain.cmake
```
```bash
cmake --build . -j4
```

## Dependencies
- CMake 3.22
- Conan (Python 3.10)
- Compilers Clang/GCC

## API Reference

**GET** localhost:18080/user
| Parameter | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `oid`     | `string` | http param                 |

**POST** localhost:18080/user
| Parameter | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `oid`     | `string` | http param                 |
| `body`    | `json`   | json data user             |

**PUT** localhost:18080/user
| Parameter | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `oid`     | `string` | http param                 |
| `body`    | `json`   | json data user             |

**DELETE** localhost:18080/user
| Parameter | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `oid`     | `string` | http param                 |

User model example raw body json:
```json
{
    "age": 26,
    "name": "miroslaw",
    "nick": "witcherofthorns",
    "photo": "http://domain/img/file.jpg"
}
```

## FAQ
#### Why didn't you use CrowCpp from Conan?
- I couldn't use CrowCpp from Conan because I'm using the new latest version of CrowCpp v1.1.0 (Pre-Release) and it's still not available in the Conan Package Center yet. I used this particular version because it is the newest and latest version of Crow for a long time with a large number of fixes and with the transition to Asio instead of Boost

#### Why is everything written functionally and no classes are used?
- Everything is written in a functional style, because I don’t see the point of using classes in a place where there is simply no point in using them, because the behavior of mongodb or crowcpp is essentially pre-defined, it’s pointless. But if you really want to use classes, you can copy all the source code and create your own simple Singleton class from it