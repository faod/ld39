
add_sources(
  src/game.cpp
  src/cyclist.cpp
  src/map.cpp
  src/food.cpp
  src/menu.cpp
  src/utilities.cpp
  src/tmx/tmx.c
  src/tmx/tmx_err.c
  src/tmx/tmx_hash.c
  src/tmx/tmx_mem.c
  src/tmx/tmx_utils.c
  src/tmx/tmx_xml.c
  src/tmx/tsx.c
)

include(FindLibXml2)
find_package(LibXml2 REQUIRED)
list(APPEND Vivace_Include_Dirs ${LIBXML2_INCLUDE_DIR})
list(APPEND Vivace_Libraries ${LIBXML2_LIBRARIES})

include(FindZLIB)
find_package(ZLIB REQUIRED)
list(APPEND Vivace_Include_Dirs ${ZLIB_INCLUDE_DIR})
list(APPEND Vivace_Libraries ${ZLIB_LIBRARIES})
list(APPEND Vivace_Definitions "WANT_ZLIB")
