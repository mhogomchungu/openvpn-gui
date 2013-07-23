 cmake_minimum_required(VERSION 2.6)  
 
 add_executable( openvpn-cli openvpn_cli.c process.c  )
 
 set_target_properties( openvpn-cli PROPERTIES COMPILE_FLAGS "-D_FILE_OFFSET_BITS=64 -Wextra -Wall -s -fPIE -pthread  -pedantic -std=c99 " )
  
 TARGET_LINK_LIBRARIES( openvpn-cli pthread )
  
 INCLUDE(FindPkgConfig)
 
 find_package( Qt4 COMPONENTS QtCore QtGui REQUIRED )
 
 find_package( KDE4 QUIET ) 
 
 INCLUDE( ${QT_USE_FILE} )
 
 INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} )
  
 add_definitions( -I${KDE4_INCLUDE_DIR} )
 add_definitions( -I${Qt4_INCLUDE_DIR} )
 add_definitions( -I${QT_HEADERS_DIR} )
 
 QT4_WRAP_UI( UI logwindow.ui )
 
 QT4_WRAP_CPP( MOC logwindow.h openvpn.h ) 
 
 add_executable( openvpn-gui main.cpp openvpn.cpp logwindow.cpp ${MOC} ${UI} )
 set_target_properties( openvpn-gui PROPERTIES COMPILE_FLAGS "-D_FILE_OFFSET_BITS=64 -Wextra -Wall -s -fPIE -pthread  -pedantic " )
 
 set_target_properties( openvpn-gui PROPERTIES COMPILE_FLAGS "-D_FILE_OFFSET_BITS=64 -Wextra -Wall -s -fPIE -pthread  -pedantic " )
  
 TARGET_LINK_LIBRARIES( openvpn-gui ${QT_LIBRARIES} -L${KDE4_LIB_DIR} -lkdeui -lkdecore )
 
 install( TARGETS openvpn-cli RUNTIME DESTINATION bin 
		PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE SETUID
 )
  
 install( TARGETS openvpn-gui RUNTIME DESTINATION bin 
		PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE 
 )