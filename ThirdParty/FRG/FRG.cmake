set(LIBS_LEARNING ${LIBS_LEARNING} frg)
set(LEARNING_TRACKERS ${LEARNING_TRACKERS} FRG/src/FRG)
set(THIRD_PARTY_INCLUDE_DIRS ${THIRD_PARTY_INCLUDE_DIRS} FRG/include)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR})
