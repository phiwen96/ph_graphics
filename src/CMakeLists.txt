file(GLOB HEADER_LIST CONFIGURE_DEPENDS "../include/${CMAKE_PROJECT_NAME}/*.hpp")

foreach(file ${HEADER_LIST})
    source_group (TREE include/${CMAKE_PROJECT_NAME}
        PREFIX "include"               
        FILES  ${file}
    )
endforeach()


file(GLOB SOURCE_LIST CONFIGURE_DEPENDS "*.cpp")

foreach(file ${SOURCE_LIST})
    source_group (TREE ${CMAKE_CURRENT_LIST_DIR}
        PREFIX "src"               
        FILES  ${file}
    )
endforeach()



add_library (lib
	${SOURCE_LIST}  ${HEADER_LIST} 
	# ALIAS ${CMAKE_PROJECT_NAME}::${CMAKE_PROJECT_NAME}
)

set_target_properties (
	lib 

	PROPERTIES
  		CXX_STANDARD 20
  		CXX_STANDARD_REQUIRED ON
  		CXX_EXTENSIONS OFF

	FOLDER
		""
)
# Make override a feature requirement only if available
target_compile_features (lib 
	PUBLIC
  		$<$<COMPILE_FEATURES:cxx_override>:cxx_override>
)
# Define the foo_OVERRIDE symbol so it provides the
# override keyword if available or empty otherwise
target_compile_definitions (lib 
	PUBLIC
  		$<$<COMPILE_FEATURES:cxx_override>:-D${CMAKE_PROJECT_NAME}_OVERRIDE=override>
  		$<$<NOT:$<COMPILE_FEATURES:cxx_override>>:-D${CMAKE_PROJECT_NAME}_OVERRIDE>

	PRIVATE 
		"$<$<CONFIG:DEBUG>:DEBUG> $<$<CONFIG:RELEASE>:RELEASE>"
)


target_compile_definitions (lib PRIVATE "$<$<CONFIG:DEBUG>:DEBUG> $<$<CONFIG:RELEASE>:RELEASE>")


# target_sources (ph_concepts PRIVATE in/platform.hpp)
set (self ${CMAKE_PROJECT_NAME})

set_target_properties (lib PROPERTIES CXX_STANDARD 20)
target_compile_features (lib PUBLIC cxx_std_20)

ph_precompile (TARGET lib PRIVATE PCH ${std_pch})

# add_library (${CMAKE_PROJECT_NAME} ALIAS ${CMAKE_PROJECT_NAME}::${CMAKE_PROJECT_NAME})
# target_link_libraries (ph_concepts PRIVATE ${Boost_LIBRARIES})
target_include_directories (lib PUBLIC ${PROJECT_SOURCE_DIR}/include)
# target_include_directories (ph_concepts PRIVATE ${Boost_INCLUDE_DIRS})

target_compile_features (lib PUBLIC cxx_std_20)

# target_compile_options(ph_concepts PUBLIC "-ast-dump  -fsyntax-only")


target_include_directories(lib PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../include/${CMAKE_PROJECT_NAME}>
    $<INSTALL_INTERFACE:include/${CMAKE_PROJECT_NAME}>
)

target_include_directories(lib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../include>
    $<INSTALL_INTERFACE:include>
)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # set_target_properties (lib PROPERTIES FOLDER "lib/")
else ()
    set_target_properties (lib PROPERTIES FOLDER "dependencies/${CMAKE_PROJECT_NAME}")
endif ()



# IDE()