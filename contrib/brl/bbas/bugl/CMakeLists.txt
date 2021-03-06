# brl/bbas/bugl/CMakeLists.txt
# written by Kongbin Kang for creating a Brown uncertainty geometry library.

doxygen_add_library(contrib/brl/bbas/bugl
  DEPENDS contrib/gel/vdgl core/vnl core/vgl core/vbl
  PACKAGE contrib/brl
  )

INCLUDE_DIRECTORIES(${BRL_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${BRL_INCLUDE_DIR}/bbas )

SET(bugl_sources
    bugl_uncertainty_point_2d.h
    bugl_gaussian_point_2d.h     bugl_gaussian_point_2d.txx
    bugl_normal_point_2d.h
    bugl_normal_point_2d_sptr.h

    bugl_uncertainty_point_3d.h
    bugl_gaussian_point_3d.h     bugl_gaussian_point_3d.txx
    bugl_normal_point_3d.h
    bugl_normal_point_3d_sptr.h

    bugl_curve_3d.h             bugl_curve_3d.cxx
    bugl_random_transform_set_2d.h     bugl_random_transform_set_2d.txx
)
AUX_SOURCE_DIRECTORY(Templates bugl_sources)

ADD_LIBRARY(bugl ${bugl_sources})
INSTALL_TARGETS(/lib bugl)
INSTALL_NOBASE_HEADER_FILES(/include/vxl/contrib/brl/bbas/bugl ${bugl_sources})
TARGET_LINK_LIBRARIES(bugl vnl vgl_algo vgl vbl)

IF(BUILD_TESTING)
  add_subdirectory(tests)
ENDIF(BUILD_TESTING)
