# chatroom

Simple multi-client chat application

In directory `build/`:

Build dependency:

```
conan install .. --build=missing
```

Build:

```
cmake .. -DCMAKE_TOOLCHAIN_FILE=build/Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

Start server: 

```
./bin/server 50505
```

Start clieng:

```
./bin/client 127.0.0.1 50505
```