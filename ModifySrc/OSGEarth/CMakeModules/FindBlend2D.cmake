
find_path(BLEND2D_INCLUDES
    blend2d.h)

find_library(BLEND2D_LIBRARY NAMES blend2d)
find_library(BLEND2D_LIBRARY_DEBUG NAMES blend2dd)

if(BLEND2D_INCLUDES AND BLEND2D_LIBRARY)
  set(BLEND2D_FOUND YES)
endif()
