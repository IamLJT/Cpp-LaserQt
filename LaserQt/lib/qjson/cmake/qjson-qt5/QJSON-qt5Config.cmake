GET_FILENAME_COMPONENT(myDir ${CMAKE_CURRENT_LIST_FILE} PATH)

SET(QJSON_LIBRARIES qjson)
SET(QJSON_INCLUDE_DIR "/home/summy/Cpp-LaserQt/LaserQt/include")

include(${myDir}/QJSON-qt5Targets.cmake)
