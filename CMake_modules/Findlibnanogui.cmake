find_path(NANOGUI_INCLUDE_DIR
    NAMES libnanogui nanogui libnanogui.h nanogui.h
    PATH_SUFFIXES nanogui)
IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
    find_library(NANOGUI_LIBRARY NAMES libnanogui_d nanogui_d libnanogui_d.so nanogui_d.so)
#ELSE(CMAKE_BUILD_TYPE MATCHES DEBUG)
#    find_library(NANOGUI_LIBRARY NAMES libnanogui nanogui libnanogui.so nanogui.so)
ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(nanogui  DEFAULT_MSG
                                  NANOGUI_LIBRARY NANOGUI_INCLUDE_DIR)

mark_as_advanced(NANOGUI_INCLUDE_DIR NANOGUI_LIBRARY )

set(NANOGUI_LIBRARIES ${NANOGUI_LIBRARY} )
set(NANOGUI_INCLUDE_DIRS ${NANOGUI_INCLUDE_DIR} )
