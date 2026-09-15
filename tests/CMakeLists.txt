add_subdirectory(camera)

if(BUILD_TESTING AND VAP_BUILD_ANALYTICS)
    add_subdirectory(analytics)
endif()
