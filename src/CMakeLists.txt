set(
	MATERIALS_SRC
	materials/material.cpp
)

set(
	CAMERA_SRC
	objects/camera/camera.cpp
)

set(
	HITABLES_SRC
	objects/hitables/box.cpp
	objects/hitables/bvh_node.cpp
	objects/hitables/hitable.cpp
	objects/hitables/hitable_list.cpp
	objects/hitables/mesh.cpp
	objects/hitables/model.cpp
	objects/hitables/rect.cpp
	objects/hitables/sphere.cpp
	objects/hitables/triangle.cpp
)

set(
	RENDER_PROCESS_SRC
	render_process/lighting_integrator.cpp
	render_process/render_worker.cpp
	render_process/tile_pool.cpp
)

set(
	SAMPLING_SRC
	render_process/sampling/random_sampler.cpp
	render_process/sampling/sampler.cpp
	render_process/sampling/stratified_sampler.cpp
)

set(
	TEXTURES_SRC
	textures/perlin.cpp
	textures/texture.cpp
)

set(
	BOUNDING_BOX_SRC
	util/bounding_box/aabb.cpp
)

set(
	DENOISER_SRC
	util/denoiser/denoiser.cpp
)

set(
	IMAGE_SRC
	util/image/image.cpp
)

set(
	MATH_SRC
	util/math/onb.cpp
	util/math/vec3.cpp
)

set(
	RNG_SRC
	util/rng/random_number.cpp
)

set(
	SCENE_PARSER_SRC
	util/scene_parser/scene_parser.cpp
)

set(
	MAIN_SRC
	"Source.cpp"
)

set(
	CPP_SOURCES
	${MATERIALS_SRC}
	${CAMERA_SRC}
	${HITABLES_SRC}
	${RENDER_PROCESS_SRC}
	${SAMPLING_SRC}
	${TEXTURES_SRC}
	${BOUNDING_BOX_SRC}
	${DENOISER_SRC}
	${IMAGE_SRC}
	${MATH_SRC}
	${RNG_SRC}
	${SCENE_PARSER_SRC}
	${MAIN_SRC}
)

	if (UNIX AND NOT APPLE)
		link_directories("${PROJECT_SOURCE_DIR}/dependencies/$<CONFIG>/linux/")
	endif (UNIX AND NOT APPLE)

	add_executable(RayTracer ${CPP_SOURCES})
	add_custom_command(TARGET RayTracer PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/$<CONFIG>)

	if (WIN32)
		target_link_libraries(
			RayTracer
			"${PROJECT_SOURCE_DIR}/dependencies/lib/$<CONFIG>/assimp-vc142-mtd.lib"
			"${PROJECT_SOURCE_DIR}/dependencies/lib/$<CONFIG>/OpenImageDenoise.lib"
		)
		add_custom_command(
			TARGET RayTracer
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/dependencies/$<CONFIG>/OpenImageDenoise.dll" "${PROJECT_SOURCE_DIR}/$<CONFIG>/OpenImageDenoise.dll"
		)
		add_custom_command(
			TARGET RayTracer
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/dependencies/$<CONFIG>/tbb.dll" "${PROJECT_SOURCE_DIR}/$<CONFIG>/tbb.dll"
		)
		add_custom_command(
			TARGET RayTracer
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/dependencies/$<CONFIG>/assimp-vc142-mtd.dll" "${PROJECT_SOURCE_DIR}/$<CONFIG>/assimp-vc142-mtd.dll"
		)
	endif (WIN32)

	if (UNIX AND NOT APPLE)
		target_link_libraries(
			RayTracer
			libassimp.so
			libOpenImageDenoise.so
			libtbb.so
			libtbbmalloc.so
			libtbbmalloc_proxy.so
			pthread
		)
		add_custom_command(
			TARGET RayTracer
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/dependencies/$<CONFIG>/linux/libOpenImageDenoise.so" "${PROJECT_SOURCE_DIR}/$<CONFIG>/libOpenImageDenoise.so"
		)
		add_custom_command(
			TARGET RayTracer
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/dependencies/$<CONFIG>/linux/libOpenImageDenoise.so.0" "${PROJECT_SOURCE_DIR}/$<CONFIG>/libOpenImageDenoise.so.0"
		)
		add_custom_command(
			TARGET RayTracer
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/dependencies/$<CONFIG>/linux/libOpenImageDenoise.so.1.1.0" "${PROJECT_SOURCE_DIR}/$<CONFIG>/libOpenImageDenoise.so.1.1.0"
		)
		add_custom_command(
			TARGET RayTracer
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/dependencies/$<CONFIG>/linux/libtbb.so" "${PROJECT_SOURCE_DIR}/$<CONFIG>/libtbb.so"
		)
		add_custom_command(
			TARGET RayTracer
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/dependencies/$<CONFIG>/linux/libtbbmalloc.so" "${PROJECT_SOURCE_DIR}/$<CONFIG>/libtbbmalloc.so"
		)
		add_custom_command(
			TARGET RayTracer
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/dependencies/$<CONFIG>/linux/libtbbmalloc_proxy.so" "${PROJECT_SOURCE_DIR}/$<CONFIG>/libtbbmalloc_proxy.so"
		)
		add_custom_command(
			TARGET RayTracer
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/dependencies/$<CONFIG>/linux/libassimp.so" "${PROJECT_SOURCE_DIR}/$<CONFIG>/libassimp.so"
		)
		add_custom_command(
			TARGET RayTracer
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/dependencies/$<CONFIG>/linux/libassimp.so.5" "${PROJECT_SOURCE_DIR}/$<CONFIG>/libassimp.so.5"
		)
		add_custom_command(
			TARGET RayTracer
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/dependencies/$<CONFIG>/linux/libassimp.so.5.0.1" "${PROJECT_SOURCE_DIR}/$<CONFIG>/libassimp.so.5.0.1"
		)
	endif (UNIX AND NOT APPLE)

	add_custom_command(
		TARGET RayTracer
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:RayTracer>" "${PROJECT_SOURCE_DIR}/$<CONFIG>/$<TARGET_FILE_NAME:RayTracer>"
	)

target_include_directories(
	RayTracer PUBLIC
	${DEPENDENCIES_DIR}/include
	${PROJECT_SOURCE_DIR}/src
)
