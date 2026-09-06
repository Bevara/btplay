cmake_minimum_required(VERSION 3.7)
project(Filters)

include(filters.cmake)

add_definitions(-fpic)

# GPAC's own scene loader, built as a side module: it is what reads the textual
# scene formats - MPEG-4 BIFS in BT or XMT form, VRML97 and X3D - and feeds the
# compositor. Without it the session reports "Cannot find filter for service"
# on a .bt, .wrl or .x3d and retries forever.
# clock.c is the same file bifsdec carries: the compositor's clock helpers
# (gf_clock_set_time, gf_clock_diff, gf_timestamp_to_clocktime) are not exported
# by the solvers, so each side module that touches a media clock brings them.
SET(BTPLAY_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/load_bt_xmt.c
        ${CMAKE_CURRENT_SOURCE_DIR}/clock.c
        ${CMAKE_CURRENT_SOURCE_DIR}/btplay_stubs.c
)

SET(BTPLAY_INC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_filter(btplay
        "${BTPLAY_SRC}"
        ""
        []
        ""
        "${BTPLAY_INC}"
        ""
        "1")
