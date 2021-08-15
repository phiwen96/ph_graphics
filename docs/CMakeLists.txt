file(GLOB DOCS_LIST CONFIGURE_DEPENDS "*.md")

foreach(file ${DOCS_LIST})
    source_group (TREE ${CMAKE_CURRENT_LIST_DIR}
        PREFIX "docs"               
        FILES  ${file}
    )
endforeach()

add_custom_target (docs
	DEPENDS 

)