set(PBD_DEMOS
    BarDemo
	test
)

if (NOT PBD_LIBS_ONLY)
	foreach (_demo_name ${PBD_DEMOS})
		option(Build_${_demo_name} "Build ${_demo_name}"	ON)
		if (Build_${_demo_name})
			add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/${_demo_name})
		endif (Build_${_demo_name})
	endforeach ()
endif()