
file(GLOB SOURCE_LIST CONFIGURE_DEPENDS "*.cpp")

foreach(file ${SOURCE_LIST})
    source_group (TREE ${CMAKE_CURRENT_LIST_DIR}
        PREFIX "src"               
        FILES  ${file}
    )
endforeach()

add_executable (app ${SOURCE_LIST})

target_link_libraries (app PRIVATE lib)

set_target_properties (
	app 

	PROPERTIES
  		CXX_STANDARD 20
  		CXX_STANDARD_REQUIRED ON
  		CXX_EXTENSIONS OFF

	FOLDER
		""
)

# set_target_properties(app PROPERTIES FOLDER "pp")


