include_directories(${CMAKE_SOURCE_DIR})

if(WASM_BUILD)
    add_subdirectory(wasm)
else()
    add_subdirectory(dispatcher)
    add_subdirectory(database)
    add_subdirectory(collection)
    add_subdirectory(wal)
    add_subdirectory(disk)
endif()