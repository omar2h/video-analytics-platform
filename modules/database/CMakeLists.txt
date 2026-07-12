add_library(database STATIC
    include/vap/database/database.hpp

    src/database.cpp
)

target_include_directories(database
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(database
    PUBLIC
        Qt6::Core
        Qt6::Sql
)
