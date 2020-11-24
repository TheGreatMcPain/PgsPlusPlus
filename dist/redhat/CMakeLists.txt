configure_file(pgs++.spec.in ${CMAKE_CURRENT_SOURCE_DIR}/pgs++.spec @ONLY)
configure_file(pack_for_rpm.sh.in ${CMAKE_CURRENT_SOURCE_DIR}/pack_for_rpm.sh @ONLY)
execute_process(COMMAND chmod 744 pack_for_rpm.sh)
execute_process(COMMAND chmod +x pack_for_rpm.sh)