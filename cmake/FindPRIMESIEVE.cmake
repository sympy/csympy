find_path(PRIMESIEVE_INCLUDE_DIR primesieve.hpp)
find_library(PRIMESIEVE_LIBRARY primesieve)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PRIMESIEVE DEFAULT_MSG PRIMESIEVE_INCLUDE_DIR
    PRIMESIEVE_LIBRARY)
