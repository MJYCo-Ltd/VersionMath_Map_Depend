include(CMakeFindDependencyMacro)

find_package(Vulkan 1.1.70.0 REQUIRED)
find_dependency(Threads)


include("${CMAKE_CURRENT_LIST_DIR}/vsgTargets.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/vsgMacros.cmake")
