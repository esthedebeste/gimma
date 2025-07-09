cmake_minimum_required(VERSION 3.10.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(gimma)

# Creates C resources file from files in given directory - thank you very much https://stackoverflow.com/a/27206982
function(create_resources dir output header)
    # Create empty output file
    file(WRITE ${output} "")
    file(WRITE ${header} "")
    # Collect input files
    file(GLOB bins ${dir}/*)
    # Iterate through input files
    foreach (bin ${bins})
        # Get short filename
        string(REGEX MATCH "([^/]+)$" filename ${bin})
        # Replace filename spaces & extension separator for C compatibility
        string(REGEX REPLACE "\\.| |-" "_" filename ${filename})
        # Read hex data from file
        file(READ ${bin} filedata HEX)
        # Convert hex data for C compatibility
        string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," filedata ${filedata})
        # Append data to output file
        file(APPEND ${output} "const unsigned char RESOURCES_${filename}[] = {${filedata}};\nconst unsigned RESOURCES_${filename}_size = sizeof(RESOURCES_${filename});\n")
        file(APPEND ${header} "extern \"C\" const unsigned char RESOURCES_${filename}[];\nextern \"C\" const unsigned RESOURCES_${filename}_size;\n")
    endforeach ()
endfunction()

# Create resources file from assets
create_resources(assets ${CMAKE_CURRENT_BINARY_DIR}/resources.c ${CMAKE_CURRENT_BINARY_DIR}/include/resources.h)

add_subdirectory(deps/simple-2d)
file(GLOB_RECURSE gimmasrc src/*.cpp ${CMAKE_CURRENT_BINARY_DIR}/resources.c ${CMAKE_CURRENT_BINARY_DIR}/include/resources.h)
add_executable(gimma ${gimmasrc})
target_link_libraries(gimma PRIVATE simple-2d)
target_include_directories(gimma PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/include)