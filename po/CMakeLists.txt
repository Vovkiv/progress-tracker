find_program(MSGMERGE msgmerge REQUIRED)
find_program(MSGFTM msgfmt REQUIRED)

set(POT_FILENAME ${CMAKE_CURRENT_SOURCE_DIR}/progress-tracker.pot)
set(MO_FILENAME progress-tracker.mo)

set(LOCALE_DIRECTORY ${PROJECT_BINARY_DIR}/locales)
file(MAKE_DIRECTORY ${LOCALE_DIRECTORY})

list(APPEND PO_NAMES bg es it nl pt_BR ru_RU tr uk_UA)

foreach(PO_NAME IN LISTS PO_NAMES)
	list(APPEND PO_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${PO_NAME}.po)
endforeach()

# Updates all the translation files whenever
foreach(PO_SOURCE PO_NAME IN ZIP_LISTS PO_SOURCES PO_NAMES)
    add_custom_command(
        OUTPUT ${PO_SOURCE}
        COMMAND msgmerge -U ${PO_SOURCE} ${POT_FILENAME}
        DEPENDS ${POT_FILENAME}
    )
    add_custom_target("update_${PO_NAME}" DEPENDS ${PO_SOURCE})
    add_dependencies(progress-tracker "update_${PO_NAME}")
endforeach()

# Compiles all the translation files onto locales directory
foreach(PO_SOURCE PO_NAME IN ZIP_LISTS PO_SOURCES PO_NAMES)
    file(MAKE_DIRECTORY ${LOCALE_DIRECTORY}/${PO_NAME}/LC_MESSAGES/)
    add_custom_command(
        OUTPUT ${LOCALE_DIRECTORY}/${PO_NAME}/LC_MESSAGES/${MO_FILENAME}
        COMMAND msgfmt --output-file=${LOCALE_DIRECTORY}/${PO_NAME}/LC_MESSAGES/${MO_FILENAME}
                      ${PO_SOURCE}
        DEPENDS ${PO_SOURCE}
    )
    add_custom_target("compile_${PO_NAME}" DEPENDS ${LOCALE_DIRECTORY}/${PO_NAME}/LC_MESSAGES/${MO_FILENAME})
    add_dependencies(progress-tracker "compile_${PO_NAME}")
endforeach()

foreach(PO_NAME IN LISTS PO_NAMES)
    if (${FLATPAK})
        install(FILES ${LOCALE_DIRECTORY}/${PO_NAME}/LC_MESSAGES/${MO_FILENAME}
                DESTINATION /app/share/locale/${PO_NAME}/LC_MESSAGES)
    else()
        install(FILES ${LOCALE_DIRECTORY}/${PO_NAME}/LC_MESSAGES/${MO_FILENAME}
                DESTINATION /usr/share/locale/${PO_NAME}/LC_MESSAGES)
    endif()
endforeach()
