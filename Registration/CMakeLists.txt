
set(Registration_SRC Registration.cxx)
generateclp(Registration_SRC Registration.xml)

## Build stand-alone command line module.
add_executable(${MYHAWKID}Registration ${Registration_SRC})
target_link_libraries(${MYHAWKID}Registration ${ITK_LIBRARIES})
