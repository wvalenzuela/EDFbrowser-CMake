CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
if (POLICY CMP0020)
    cmake_policy(SET CMP0020 NEW)
endif ()
if (POLICY CMP0043)
    cmake_policy(SET CMP0043 NEW)
endif ()
if (POLICY CMP0042)
    cmake_policy(SET CMP0042 NEW)
endif ()
if (POLICY CMP0043)
    cmake_policy(SET CMP0043 NEW)
endif ()
IF(POLICY CMP0071)
    cmake_policy(SET CMP0071 NEW)
ENDIF()
# ======================================================================
# ======================================================================
PROJECT(EDFbrowser)
# ======================================================================
SET(APP_NAME )
STRING(TOUPPER ${PROJECT_NAME} APP_NAME)
MESSAGE("\n------------------------${PROJECT_NAME}------------------------")
# ======================================================================
SET(${APP_NAME}_VERSION_MAJOR 1)
SET(${APP_NAME}_VERSION_MINOR 68)
SET(${APP_NAME}_VERSION_PATCH 0)
SET(${APP_NAME}_VERSION_COUNT 0)
SET(${APP_NAME}_VERSION "${${APP_NAME}_VERSION_MAJOR}.${${APP_NAME}_VERSION_MINOR}.${${APP_NAME}_VERSION_PATCH}.${${APP_NAME}_VERSION_COUNT}" CACHE INTERNAL "")
# ===================================================================================
MESSAGE("Project Name: EDFbrowser, Version: ${${APP_NAME}_VERSION}")
# ===================================================================================
# ======================================================================
IF (APPLE)
    SET(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-register")
    IF (XCODE_VERSION GREATER 7.1)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-inconsistent-missing-override")
    ENDIF (XCODE_VERSION GREATER 7.1)
    FIND_LIBRARY(LIB_COCOA Cocoa)
    FIND_LIBRARY(LIB_FOUNDATION Foundation)
    SET(APPLE_LIBS ${LIB_COCOA} ${LIB_FOUNDATION})
	SET(CMAKE_SHARED_LINKER_FLAGS "-Wl,--export-all-symbols")
ELSEIF (WIN32)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MD")
    SET(GUI_TYPE WIN32)
ELSEIF (UNIX AND NOT APPLE)
	SET(CMAKE_CXX_FLAGS                " ${CMAKE_CXX_FLAGS_INIT} -std=c++11 -fPIC")
	SET(CMAKE_CXX_FLAGS_DEBUG          "-g -DDEBUG")
	SET(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
	SET(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DDEBUG")
ENDIF (APPLE)
# ======================================================================
if(CMAKE_COMPILER_IS_GNUCXX)
    EXECUTE_PROCESS(COMMAND ${CMAKE_C_COMPILER} -dumpversion
                    OUTPUT_VARIABLE GCC_VERSION)
    if(GCC_VERSION VERSION_GREATER 4.8 OR GCC_VERSION VERSION_EQUAL 4.8)
        set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
    else(GCC_VERSION VERSION_GREATER 4.8 OR GCC_VERSION VERSION_EQUAL 4.8)
        set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")
    endif(GCC_VERSION VERSION_GREATER 4.8 OR GCC_VERSION VERSION_EQUAL 4.8)
endif(CMAKE_COMPILER_IS_GNUCXX)

if(CMAKE_CXX_COMPILER MATCHES "clang")
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -Wformat-nonliteral")
    set(CMAKE_C_FLAGS    "${CMAKE_C_FLAGS} -Wformat-nonliteral")
endif(CMAKE_CXX_COMPILER MATCHES "clang")


if("${CMAKE_C_COMPILER_ID}" MATCHES "GNU")
	set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wcast-align -Werror-implicit-function-declaration -Wchar-subscripts -Wall -W")
	set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wpointer-arith -Wwrite-strings -Wformat-security -Wmissing-format-attribute -fno-common")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnon-virtual-dtor -ansi -Wcast-align -Wchar-subscripts -Wall -W -Wshadow")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpointer-arith -Wformat-security")
	if (NOT APPLE)
		set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--fatal-warnings -Wl,--no-undefined ${CMAKE_SHARED_LINKER_FLAGS}")
		set(CMAKE_MODULE_LINKER_FLAGS "-Wl,--fatal-warnings -Wl,--no-undefined ${CMAKE_MODULE_LINKER_FLAGS}")
	endif ()
endif()

# Must be added to enable export macro
	
#
# Check to see if we have large file support
#
if (APPLE)
  add_definitions(-D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64)
  add_definitions(-DNCBI_XCODE_BUILD -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64)
elseif (UNIX)
  add_definitions(-D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64)
endif (APPLE)

SET(CMAKE_REQUIRED_DEFINITIONS -D_LARGEFILE64_SOURCE=1)
# We add these other definitions here because CheckTypeSize.cmake
# in CMake 2.4.x does not automatically do so and we want
# compatibility with CMake 2.4.x.
IF(HAVE_SYS_TYPES_H)
    LIST(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_SYS_TYPES_H)
ENDIF()
IF(HAVE_STDINT_H)
    LIST(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_STDINT_H)
ENDIF()
if(HAVE_STDDEF_H)
    LIST(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_STDDEF_H)
ENDIF()
INCLUDE (CheckTypeSize)
check_type_size(off64_t OFF64_T)
IF(HAVE_OFF64_T)
   ADD_DEFINITIONS(-D_LARGEFILE64_SOURCE=1)
ENDIF()
SET(CMAKE_REQUIRED_DEFINITIONS) # clear variable
		
# ======================================================================
# ======================================================================
# Instruct CMake to Include OpenGL.
FIND_PACKAGE(OpenGL)
# ======================================================================
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
SET(CMAKE_AUTOMOC ON)

# ======================================================================
SET(QT_BASE_DIRECTORY " "  CACHE PATH "Qt base Directory")
IF(NOT EXISTS ${QT_BASE_DIRECTORY})
	MESSAGE(FATAL_ERROR "\nQt base directory error please define the directory")
ENDIF()

# Instruct CMake to Include Qt5.
SET(QT_COMPONENTS_REQUIRED Core Gui Widgets OpenGL OpenGLExtensions Concurrent Sql Network PrintSupport)
FIND_PACKAGE(Qt5 REQUIRED COMPONENTS ${QT_COMPONENTS_REQUIRED})

#define qt libraries to use
SET(QT_LIBRARIES Qt5::Core Qt5::Gui Qt5::Widgets Qt5::OpenGL Qt5::OpenGLExtensions Qt5::Concurrent Qt5::Sql Qt5::Network Qt5::PrintSupport)

# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDES})

# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
ADD_DEFINITIONS(${Qt5Widgets_DEFINITIONS})

# Executables fail to build with Qt 5 in the default configuration
# without -fPIE. We add that here.
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS})

# Add the include directories for the Qt 5 Widgets module to
# the compile lines.
ADD_DEFINITIONS(${QT_DEFINITIONS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${QT_LIBRARY_DIRS})

SET(QT_PLUGIN_DIRECTORY ${QT_BASE_DIRECTORY}/plugins)
SET(QT_BIN_DIRECTORY ${QT_BASE_DIRECTORY}/bin)
SET(QT_LIB_DIRECTORY ${QT_BASE_DIRECTORY}/lib)
# ======================================================================
# ======================================================================

SET(main_source
    main.cpp
    mainwindow.cpp
    mainwindow_constr.cpp
)
SET(main_header
    mainwindow.h
)
SOURCE_GROUP("Source Files\\Main" FILES ${main_source})
SOURCE_GROUP("Header Files\\Main" FILES ${main_header})

SET(edflib_source
    edflib.c 
)
SET(edflib_header 
	edflib.h 
)
SOURCE_GROUP("Source Files\\EDFLib" FILES ${edflib_source})
SOURCE_GROUP("Header Files\\EDFLib" FILES ${edflib_header})

SET(others_source
	viewbuf.cpp
	videoplayer.cpp
	read_write_settings.cpp
	viewcurve.cpp
	check_edf_file.cpp
	show_edf_hdr.cpp
	signals_dialog.cpp
	signal_chooser.cpp
	ascii_export.cpp
	edf_compat.cpp
	colordialog.cpp
	filter.c
	filter_dialog.cpp
	jump_dialog.cpp
	about_dialog.cpp
	edf_annotations.cpp
	annotations_dock.cpp
	options_dialog.cpp
	nk2edf.cpp
	xml.c
	save_montage_dialog.cpp
	load_montage_dialog.cpp
	view_montage_dialog.cpp
	show_actual_montage_dialog.cpp
	print_to_edf.cpp
	pagetime_dialog.cpp
	print_to_bdf.cpp
	ascii2edf.cpp
	fino2edf.cpp
	nexfin2edf.cpp
	edfplusd_cnv.cpp
	utc_date_time.c
	emsa2edf.cpp
	special_button.cpp
	bdf2edf.cpp
	edit_annotation_dock.cpp
	popup_save_cancelwindow.cpp
	save_annots.cpp
	filtercurve.cpp
	utils.c
	signalcurve.cpp
	spectrumanalyzer.cpp
	bi9800.cpp
	export_annotations.cpp
	edit_predefined_mtg.cpp
	spectrum_dock.cpp
	edf_annot_list.c
	reduce_signals.cpp
	active_file_chooser.cpp
	header_editor.cpp
	biosemi2bdfplus.cpp
	bdf_triggers.cpp
	adjustfiltersettings.cpp
	import_annotations.cpp
	ravg_filter.c
	wav2edf.cpp
	fma_ecg2edf.cpp
	mortara2edf.cpp
	averager_dialog.cpp
	averager_curve_wnd.cpp
	ecg_filter.c
	ecg_export.cpp
	statistics_dialog.cpp
	filteredblockread.cpp
	flywheel.cpp
	z_score_dialog.cpp
	z_ratio_filter.c
	raw2edf.cpp
	check_for_updates.cpp
	manscan2edf.cpp
	scp_ecg2edf.cpp
	unisens2edf.cpp
	date_time_stamp_parser.c
	spike_filter.c
	spike_filter_dialog.cpp
	mit2edf.cpp
	biox2edf.cpp
	edf_helper.c
	plif_ecg_subtract_filter.c
	plif_ecg_subtract_filter_dialog.cpp
	annotlist_filter_dialog.cpp
	export_filtered_signals.cpp
	fft_wrap.c
	ecg_statistics.c
	fir_filter.c
	fir_filter_dialog.cpp
)
SET(others_header
	global.h
	viewcurve.h
	check_edf_file.h
	show_edf_hdr.h
	signals_dialog.h
	signal_chooser.h
	ascii_export.h
	edf_compat.h
	colordialog.h
	filter.h
	filter_dialog.h
	jump_dialog.h
	about_dialog.h
	edf_annotations.h
	annotations_dock.h
	options_dialog.h
	nk2edf.h
	xml.h
	save_montage_dialog.h
	load_montage_dialog.h
	view_montage_dialog.h
	show_actual_montage_dialog.h
	print_to_edf.h
	pagetime_dialog.h
	print_to_bdf.h
	ascii2edf.h
	fino2edf.h
	nexfin2edf.h
	edfplusd_cnv.h
	utc_date_time.h
	emsa2edf.h
	special_button.h
	bdf2edf.h
	edit_annotation_dock.h
	popup_save_cancelwindow.h
	save_annots.h
	filtercurve.h
	utils.h
	signalcurve.h
	spectrumanalyzer.h
	bi9800.h
	export_annotations.h
	edit_predefined_mtg.h
	spectrum_dock.h
	edf_annot_list.h
	reduce_signals.h
	active_file_chooser.h
	header_editor.h
	biosemi2bdfplus.h
	bdf_triggers.h
	adjustfiltersettings.h
	import_annotations.h
	ravg_filter.h
	wav2edf.h
	fma_ecg2edf.h
	mortara2edf.h
	averager_dialog.h
	averager_curve_wnd.h
	ecg_filter.h
	ecg_export.h
	statistics_dialog.h
	filteredblockread.h
	flywheel.h
	z_score_dialog.h
	z_ratio_filter.h
	raw2edf.h
	check_for_updates.h
	manscan2edf.h
	scp_ecg2edf.h
	unisens2edf.h
	date_time_stamp_parser.h
	spike_filter.h
	spike_filter_dialog.h
	mit2edf.h
	biox2edf.h
	edf_helper.h
	plif_ecg_subtract_filter.h
	plif_ecg_subtract_filter_dialog.h
	annotlist_filter_dialog.h
	export_filtered_signals.h
	fft_wrap.h
	ecg_statistics.h
	fir_filter.h
	fir_filter_dialog.h
)
SOURCE_GROUP("Source Files\\Others" FILES ${others_source})
SOURCE_GROUP("Header Files\\Others" FILES ${others_header})

SET(findlib_source
	third_party/fidlib/fidlib.c
)
SET(findlib_header
	third_party/fidlib/fidlib.h
	third_party/fidlib/fidmkf.h
	third_party/fidlib/fidrf_cmdlist.h
)
SOURCE_GROUP("Source Files\\Thrid Party\\Findlib" FILES ${findlib_source})
SOURCE_GROUP("Header Files\\Thrid Party\\Findlib" FILES ${findlib_header})

SET(kiss_fft_source
	third_party/kiss_fft/kiss_fft.c
	third_party/kiss_fft/kiss_fftr.c
)
SET(kiss_fft_header
	third_party/kiss_fft/kiss_fft.h
	third_party/kiss_fft/_kiss_fft_guts.h
	third_party/kiss_fft/kiss_fftr.h
)
SOURCE_GROUP("Source Files\\Thrid Party\\Kiss FFT" FILES ${kiss_fft_source})
SOURCE_GROUP("Header Files\\Thrid Party\\Kiss FFT" FILES ${kiss_fft_header})

set(FORMS
)

SET(sources
    ${main_source}
	${others_source}
	${findlib_source}
	${kiss_fft_source}
	${edflib_source}
)
SET(headers
   	${main_header}
	${others_header}
	${findlib_header}
	${kiss_fft_header}
	${edflib_header}
)
# ======================================================================
SET(RESOURCES images.qrc)
SOURCE_GROUP("Resources" FILES ${RESOURCES} ${ICON_LOCATION})

IF (APPLE)
	# set how it shows up in the Info.plist file
	SET(MACOSX_BUNDLE_ICON_FILE ${PROJECT_NAME}.icns)
	# include the icns file in the target
	SET(ICON_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.icns)
	MESSAGE("\nProject: ${PROJECT_NAME} Icon location, Source path: ${ICON_LOCATION}")
	# set where in the bundle to put the icns file
	SET_SOURCE_FILES_PROPERTIES(${ICON_LOCATION} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
ELSEIF (WIN32)
	SET(WIN_RESOURCE_FILE edfbrowser.rc)
ENDIF (APPLE)

QT5_ADD_RESOURCES(RESOURCES_SOURCES ${RESOURCES})

QT5_WRAP_UI(UISrcs ${FORMS})

MESSAGE("------Configuring the Executable of ${PROJECT_NAME}------")
# CMAKE_AUTOMOC in ON so the MocHdrs will be automatically wrapped.
ADD_EXECUTABLE(${PROJECT_NAME} WIN32 MACOSX_BUNDLE
				${ICON_LOCATION}
				${sources} ${headers} ${UISrcs} ${RESOURCES_SOURCES}
			${WIN_RESOURCE_FILE})

QT5_USE_MODULES(${PROJECT_NAME} ${QT_COMPONENTS_REQUIRED})

MESSAGE("------Configuring the Target Link of ${PROJECT_NAME}------\n")

MESSAGE("${PROJECT_NAME} Qt  libs:  ${QT_LIBRARIES}")

TARGET_LINK_LIBRARIES(${PROJECT_NAME}  ${QT_LIBRARIES} ${APPLE_LIBS})
# ===================================================================================
# ===================================================================================
IF(WIN32)
	# Properties->General->Output Directory
	SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)
# ===================================================================================
# ---------------------------------------------------------------------------
# ---------------------------------------------------------------------------
#--------------------------------------------------------------------------------
# Now the installation stuff below
#--------------------------------------------------------------------------------
SET(plugin_dest_dir)
SET(qtconf_dest_dir)

SET(APPS \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME})

SET(APP_DESTINY bin)

SET(VTK_DIRECTORIES ${VTK_DLL_DIRECTORY})
SET(ITK_LIBRARY_DIRS ${ITK_DLL_DIRECTORY})

IF (APPLE)
	SET(APPS "${APPS}.app")
	SET(plugin_dest_dir ${APPS}/Contents/MacOS)
	SET(qtconf_dest_dir ${APPS}/Contents/Resources)
	SET(APP_DESTINY bin/${PROJECT_NAME}.app/Contents/MacOS)

	SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
ELSEIF (WIN32)
	SET(APPS "${APPS}.exe")
	SET(plugin_dest_dir bin)
	SET(qtconf_dest_dir ${CMAKE_INSTALL_PREFIX}/bin)
ELSEIF (UNIX AND NOT APPLE)
    SET(qtconf_dest_dir ${CMAKE_INSTALL_PREFIX}/bin)
ENDIF (APPLE)
#--------------------------------------------------------------------------------
# Install the QtTest application, on Apple, the bundle is at the root of the
# install tree, and on other platforms it'll go into the bin directory.
INSTALL(TARGETS ${PROJECT_NAME}
		# IMPORTANT: Add the bar executable to the "export-set"
		EXPORT ${PROJECT_NAME}Targets
		BUNDLE DESTINATION bin COMPONENT ${PROJECT_NAME}
		RUNTIME DESTINATION bin COMPONENT ${PROJECT_NAME})

#--------------------------------------------------------------------------------
# Install needed Qt plugins by copying directories from the qt installation
# One can cull what gets copied by using 'REGEX "..." EXCLUDE'
INSTALL(DIRECTORY ${QT_PLUGIN_DIRECTORY}/imageformats ${QT_PLUGIN_DIRECTORY}/platforms ${QT_PLUGIN_DIRECTORY}/sqldrivers
		DESTINATION ${APP_DESTINY}/plugins
		COMPONENT ${PROJECT_NAME})
#--------------------------------------------------------------------------------
# install a qt.conf file
# this inserts some cmake code into the install script to write the file
MESSAGE("\nProject: ${PROJECT_NAME}, Source path: ${plugin_dest_dir}")
MESSAGE("Project: ${PROJECT_NAME}, Source path: ${qtconf_dest_dir}\n")
INSTALL(CODE "
				file(WRITE \"${qtconf_dest_dir}/qt.conf\" \"\")
			" 
		COMPONENT ${PROJECT_NAME})
#--------------------------------------------------------------------------------
# Use BundleUtilities to get all other dependencies for the application to work.
# It takes a bundle or executable along with possible plugins and inspects it
# for dependencies.  If they are not system dependencies, they are copied.
# directories to look for dependencies

IF (NOT CMAKE_CFG_INTDIR)
	SET(CMAKE_CFG_INTDIR "Release")
ENDIF (NOT CMAKE_CFG_INTDIR)

SET(DIRS ${QT_BIN_DIRECTORY} 
		 ${QT_LIB_DIRECTORY}
	 	 ${CMAKE_INSTALL_PREFIX}/lib
	 	${CMAKE_INSTALL_PREFIX}/bin
 )
IF (APPLE)

ELSEIF (WIN32)
	SET(DIRS ${DIRS} ${CMAKE_BINARY_DIR}/bin/Release) 
ENDIF(APPLE)
# Now the work of copying dependencies into the bundle/package
# The quotes are escaped and variables to use at install time have their $ escaped
# An alternative is the do a configure_file() on a script and use install(SCRIPT  ...).
# Note that the image plugins depend on QtSvg and QtXml, and it got those copied
# over.

INSTALL(CODE "
			FILE(GLOB_RECURSE QTPLUGINS	\"${plugin_dest_dir}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
			INCLUDE(BundleUtilities)
			FIXUP_BUNDLE(\"${APPS}\" \"\${QTPLUGINS}\" \"${DIRS}\")
		" 
		COMPONENT ${PROJECT_NAME})
#--------------------------------------------------------------------------------
#--------------------------------------------------------------------------------
# To Create a package, one can run "cpack -G DragNDrop CPackConfig.cmake" on Mac OS X
# where CPackConfig.cmake is created by including CPack
# And then there's ways to customize this as well
INCLUDE(InstallRequiredSystemLibraries)

INSTALL(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} 
		DESTINATION ${APP_DESTINY}
		COMPONENT ${PROJECT_NAME})
		
IF (APPLE)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in)
    MESSAGE("${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in")
    SET(MACOSX_BUNDLE_INFO_STRING "${PROJECT_NAME} ${PROJECT_VERSION}")
    SET(MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_NAME} ${PROJECT_VERSION}")
    SET(MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_NAME} ${PROJECT_VERSION}")
    SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}")
    SET(MACOSX_BUNDLE_COPYRIGHT "${PROJECT_COPYRIGHT_YEAR} ${PROJECT_VENDOR}")

    SET(MACOSX_BUNDLE_GUI_IDENTIFIER "${PROJECT_DOMAIN_SECOND}.${PROJECT_DOMAIN_FIRST}")
    SET(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
ELSEIF (WIN32)
	MESSAGE("Path to Git base repo: ${CMAKE_SOURCE_DIR}")

    set(CPACK_GENERATOR NSIS)
	SET(CPACK_NSIS_MODIFY_PATH ON)
	SET(CPACK_NSIS_DISPLAY_NAME "${PROJECT_NAME}")
	SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
		CreateShortcut \\\"$APPDATA\\\\Microsoft\\\\Internet\\\ Explorer\\\\Quick\\\ Launch\\\\User\\\ Pinned\\\\TaskBar\\\\${PROJECT_NAME}.lnk\\\" \\\"$INSTDIR\\\\bin\\\\${PROJECT_NAME}.exe\\\"
		CreateShortcut \\\"$QUICKLAUNCH\\\\${PROJECT_NAME}.lnk\\\" \\\"$INSTDIR\\\\bin\\\\${PROJECT_NAME}.exe\\\"
		CreateShortcut \\\"$DESKTOP\\\\${PROJECT_NAME}.lnk\\\" \\\"$INSTDIR\\\\bin\\\\${PROJECT_NAME}.exe\\\"
		CreateShortcut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${PROJECT_NAME}\\\\${UNINSTALL_SHORTCUT}.lnk\\\" \\\"$INSTDIR\\\\Uninstall.exe\\\"	
	")	
	SET(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
		Delete \\\"$APPDATA\\\\Microsoft\\\\Internet\\\ Explorer\\\\Quick\\\ Launch\\\\User\\\ Pinned\\\\TaskBar\\\\${PROJECT_NAME}.lnk\\\"
		Delete \\\"$DESKTOP\\\\${PROJECT_NAME}.lnk\\\"
		Delete \\\"$SMPROGRAMS\\\\$START_MENU\\\\${PROJECT_NAME}\\\\${UNINSTALL_SHORTCUT}.lnk\\\"
		Delete \\\"$QUICKLAUNCH\\\\${PROJECT_NAME}.lnk\\\"
	")

	SET(LIBS_EXTERNALS
		${QT_BIN_DIRECTORY}/Qt5Gui.dll
		${QT_BIN_DIRECTORY}/Qt5Widgets.dll
		${QT_BIN_DIRECTORY}/Qt5Core.dll
	)
    INSTALL(FILES ${LIBS_EXTERNALS}
            DESTINATION bin
            COMPONENT ${PROJECT_NAME})
    # By default, do not warn when built on machines using only VS Express:
    IF (NOT DEFINED CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS)
        SET(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS ON)
    ENDIF ()
	if(CMAKE_CXX_FLAGS MATCHES "/EHc$")
		message(STATUS "CMAKE_CXX_FLAGS matches /EHc at end of string --replaced...")
		message(STATUS "")
	endif()

ELSEIF (UNIX AND NOT APPLE)   
	INSTALL(DIRECTORY
		${CMAKE_CURRENT_SOURCE_DIR}/Linux/icons/
		DESTINATION ${CMAKE_INSTALL_PREFIX}/icons
	  )

	  INSTALL(FILES
		${CMAKE_CURRENT_SOURCE_DIR}/Linux/${PROJECT_NAME}.desktop
		DESTINATION ${CMAKE_INSTALL_PREFIX}/applications
	  )

	  INSTALL(FILES
		${CMAKE_CURRENT_SOURCE_DIR}/Linux/icons/hicolor/128x128/apps/${PROJECT_NAME}.png
		DESTINATION ${CMAKE_INSTALL_PREFIX}/pixmaps
	  )

    SET(CPACK_SOURCE_INSTALLED_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR};/;${CMAKE_INSTALL_PREFIX}/bin;${CMAKE_INSTALL_PREFIX}/lib")
    SET(CPACK_STRIP_FILES "bin/${PROJECT_NAME}")
    SET(CPACK_SOURCE_STRIP_FILES "")
    SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

    SET(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})

    SET(CPACK_GENERATOR "TGZ;DEB")
    # Use dkpg-shlibdeps to generate dependency list
    SET(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
    # Enable debug message
    SET(CPACK_DEBIAN_PACKAGE_DEBUG 1)

    SET(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
    SET(CPACK_DEB_COMPONENT_INSTALL 1)
    SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.3.1-6), libgcc1 (>= 1:3.4.2-12)")

	SET(CPACK_SET_DESTDIR TRUE)
	SET(CPACK_INSTALL_PREFIX $ENV{HOME}/${PROJECT_NAME})

	# use, i.e. don't skip the full RPATH for the build tree
	SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
ENDIF (APPLE)

SET(CPACK_PACKAGE_NAME ${PROJECT_NAME})
SET(CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_NAME})
SET(CPACK_PACKAGE_VERSION ${${APP_NAME}_VERSION})
SET(CPACK_PACKAGE_VERSION_MAJOR ${${APP_NAME}_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${${APP_NAME}_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${${APP_NAME}_VERSION_PATCH})

SET(CPACK_OUTPUT_FILE_PREFIX ${PROJECT_NAME})
SET(CPACK_PACKAGE_EXECUTABLES "${PROJECT_NAME}" "${PROJECT_NAME}")
SET(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}")
SET(CPACK_COMPONENTS_ALL ${PROJECT_NAME})

SET(CPACK_PACKAGE_VENDOR ${THOTH_VENDOR})
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_NAME} - Component Installation")

MESSAGE("Project Name: ${PROJECT_NAME}, Version: ${CPACK_PACKAGE_VERSION}")

INCLUDE(CPack)
