## Browser-Simulator

## Find libtidy which is needed to parse HTML
if("${TIDY_LIBRARIES}" STREQUAL "")
    find_package(TIDY REQUIRED)
endif()
if("${RT_LIBRARIES}" STREQUAL "")
    find_package(RT REQUIRED)
endif()
if("${GLIB_LIBRARIES}" STREQUAL "")
    find_package(GLIB REQUIRED)
endif()

include_directories(${CMAKE_SOURCE_DIR}/filetransfer)
include_directories(AFTER ${TIDY_INCLUDES} ${RT_INCLUDES} ${GLIB_INCLUDES})

set(browser_sources
    browser.c 
    html.c
    url.c
)

## service library to allow browser to be used by any plugin
add_library(shadow-service-browser STATIC ${browser_sources})
add_dependencies(shadow-service-browser shadow-util)
target_link_libraries(shadow-service-browser shadow-service-filetransfer ${RT_LIBRARIES} ${GLIB_LIBRARIES} ${TIDY_LIBRARIES})

## executable that can run outside of shadow
add_executable(shadow-browser browser-main.c)
target_link_libraries(shadow-browser shadow-service-filetransfer shadow-service-browser ${RT_LIBRARIES} ${GLIB_LIBRARIES} ${TIDY_LIBRARIES})
install(TARGETS shadow-browser DESTINATION bin)

## build bitcode - other plugins may use the service bitcode target
add_bitcode(shadow-service-browser-bitcode ${browser_sources})
add_bitcode(shadow-plugin-browser-bitcode browser-plugin.c)

## create and install a shared library that can plug into shadow
add_plugin(shadow-plugin-browser shadow-plugin-browser-bitcode shadow-service-browser-bitcode shadow-service-filetransfer-bitcode)
target_link_libraries(shadow-plugin-browser ${TIDY_LIBRARIES} ${GLIB_LIBRARIES})
install(TARGETS shadow-plugin-browser DESTINATION plugins)

## the following two lines are needed if we want to allow external plug-ins to use ours
if(SHADOW_EXPORT STREQUAL ON)
    install(TARGETS shadow-service-browser shadow-service-browser-bitcode EXPORT shadow-externals DESTINATION lib)
    install(DIRECTORY ./ DESTINATION include/ FILES_MATCHING PATTERN "*.h")
endif(SHADOW_EXPORT STREQUAL ON)
