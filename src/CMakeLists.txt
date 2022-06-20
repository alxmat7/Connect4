
set(HEADER_LIST "${Connect4_SOURCE_DIR}/include/Board.h" "${Connect4_SOURCE_DIR}/include/Globals.h" "${Connect4_SOURCE_DIR}/include/MiniMaxAiPlayer.h" "${Connect4_SOURCE_DIR}/include/Player.h" "${Connect4_SOURCE_DIR}/include/GameController.h" "${Connect4_SOURCE_DIR}/include/GameView.h")

message(STATUS "HEADER_LIST=${HEADER_LIST}")


add_executable(Connect4 
	Connect4.cpp 
	Board.cpp 
	Globals.cpp 
	MiniMaxAiPlayer.cpp 
	GameController.cpp
	GameView.cpp ${HEADER_LIST}
	)
	
target_include_directories(Connect4 PRIVATE ../include)
target_link_libraries(Connect4 sfml-graphics)
target_compile_features(Connect4 PUBLIC cxx_std_11)

source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})