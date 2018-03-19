find_path(EIGEN_INCLUDE_DIR
    NAMES eigen Eigen
    PATH_SUFFIXES eigen3)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(eigen  DEFAULT_MSG
                                  EIGEN_INCLUDE_DIR)

set(NANOGUI_INCLUDE_DIRS ${NANOGUI_INCLUDE_DIR} )
