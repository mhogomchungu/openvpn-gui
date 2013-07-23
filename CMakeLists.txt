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
  
 install ( FILES ${PROJECT_BINARY_DIR}/openvpn-gui.desktop DESTINATION share/applications 
 PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE 
 )
 
 install ( FILES openvpn-gui.png DESTINATION share/icons )
 
 # uninstall target
 configure_file(
	 "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
	 "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	 IMMEDIATE @ONLY )
 
 add_custom_target( uninstall
 COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake )
 
 file( WRITE "${PROJECT_BINARY_DIR}/openvpn_cli_path.h" "#define OPEN_VPN_CLI_PATH \"${CMAKE_INSTALL_PREFIX}/bin/openvpn-cli\"" )
 
 # desktop file section
 file( WRITE "${PROJECT_BINARY_DIR}/openvpn-gui.desktop"
 "#!/usr/bin/env xdg-open
 [Desktop Entry]
 Comment[en_US]=
 Comment=
 Exec=${CMAKE_INSTALL_PREFIX}/bin/openvpn-gui
 GenericName[en_US]=a GUI front end to openvpn
 GenericName=a GUI front end to openvpn
 Icon=${CMAKE_INSTALL_PREFIX}/share/icons/openvpn-gui.png
 MimeType=
 Name[en_US]=openvpn-gui
 Name=openvpn-gui
 NoDisplay=false
 Path=
 StartupNotify=true
 Terminal=false
 TerminalOptions=
 Type=Application
 X-DBUS-ServiceName=
 X-DBUS-StartupType=Unique
 X-KDE-SubstituteUID=false
 X-KDE-Username=
 Categories=X-MandrivaLinux-Internet-RemoteAccess;Network;RemoteAccess;RemoteAccess;\n" )
 