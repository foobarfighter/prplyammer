# - Try to find cometd
# Once done this will define
#
#  COMETD_FOUND - system has cometd
#  COMETD_INCLUDE_DIRS - the cometd include directory
#  COMETD_LIBRARIES - Link these to use cometd
#  COMETD_DEFINITIONS - Compiler switches required for using cometd
#
#  Copyright (c) 2014 Bob Remeika <bob.remeika@gmail.com>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#


if (COMETD_LIBRARIES AND COMETD_INCLUDE_DIRS)
  # in cache already
  set(COMETD_FOUND TRUE)
else (COMETD_LIBRARIES AND COMETD_INCLUDE_DIRS)
  # use pkg-config to get the directories and then use these values
  # in the FIND_PATH() and FIND_LIBRARY() calls
    include(UsePkgConfig)

    pkgconfig(cometd _COMETDIncDir _COMETDLinkDir _COMETDLinkFlags _COMETDCflags)

    set(COMETD_DEFINITIONS ${_COMETDCflags})

    find_path(COMETD_INCLUDE_DIR
        NAMES
            cometd.h
        PATHS
            ${_COMETDIncDir}
            /usr/local/include
    )

    find_library(COMETD_LIBRARY
        NAMES
            cometd
        PATHS
            ${_COMETDLinkDir}
            /usr/local/lib/
    )

    set(COMETD_INCLUDE_DIRS
        ${COMETD_INCLUDE_DIR}
        ${COMETD_INCLUDE_DIR}/cometd
    )

    set(COMETD_LIBRARIES
        ${COMETD_LIBRARY}
    )

  if (COMETD_INCLUDE_DIRS AND COMETD_LIBRARIES)
    set(COMETD_FOUND TRUE)
  endif (COMETD_INCLUDE_DIRS AND COMETD_LIBRARIES)

  if (COMETD_FOUND)
    if (NOT COMETD_FIND_QUIETLY)
      message(STATUS "Found cometd: ${COMETD_LIBRARIES}")
    endif (NOT COMETD_FIND_QUIETLY)
  else (COMETD_FOUND)
    if (COMETD_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find cometd")
    endif (COMETD_FIND_REQUIRED)
  endif (COMETD_FOUND)

  # show the COMETD_INCLUDE_DIRS and COMETD_LIBRARIES variables only in the advanced view
  mark_as_advanced(COMETD_INCLUDE_DIRS COMETD_LIBRARIES)

endif (COMETD_LIBRARIES AND COMETD_INCLUDE_DIRS)
