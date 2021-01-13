foreach(EXAMPLE ${EXAMPLES})
  add_executable(${EXAMPLE} WIN32)

  add_subdirectory(${EXAMPLE})
  set_target_properties(${EXAMPLE}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/examples"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/examples"
  )
  if(MINGW)
    message("MINGW")
  endif()

  target_link_libraries(${EXAMPLE} PRIVATE yaul entrypoint)
endforeach()

if (WIN32)
  foreach(EXAMPLE ${EXAMPLES})
    install(TARGETS ${EXAMPLE}
            RUNTIME DESTINATION .
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION .)
    set(EXE $<TARGET_FILE:${EXAMPLE}>)
    list(APPEND EXES ${EXE})
  endforeach()
else()
  install(TARGETS ${EXAMPLES}
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib
          ARCHIVE DESTINATION lib)
endif()
