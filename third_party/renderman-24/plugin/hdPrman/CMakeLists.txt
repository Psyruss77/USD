# Install hdPrman's headers in pxr/imaging alongside other shared
# libraries for Hydra backends, like hdSt.
set(PXR_PREFIX pxr/imaging)
set(PXR_PACKAGE hdPrman)

# RiEntryPoints.h incorrectly makes libloadprman symbols DLL import
# This is only temporary and will be removed shortly
add_definitions(-DPRMANBUILDINGAPI)

# TODO The pxr cmake macros don't provide a good way to link prman with link_directories
# on Linux/OSX without resorting to absolute rpath. For now, simply avoid linking
# libprman on Linux/OSX.
set(optionalLibs "")
if(WIN32)
    set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:libmmd.lib /NODEFAULTLIB:libirc.lib /NODEFAULTLIB:svml_dispmd.lib /NODEFAULTLIB:libdecimal.lib" )
    list(APPEND optionalLibs ${PXRCORE_LIBRARY})
    list(APPEND optionalLibs ${PRMAN_LIBRARY})
elseif(APPLE)
    set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined dynamic_lookup -headerpad_max_install_names" )
endif()

set(optionalIncludeDirs "")
set(optionalPublicClasses "")
if (${PXR_ENABLE_MATERIALX_SUPPORT})
    add_definitions(-DPXR_MATERIALX_STDLIB_DIR="${MATERIALX_STDLIB_DIR}")
    add_definitions(-DPXR_MATERIALX_BASE_DIR="${MATERIALX_BASE_DIR}")

    list(APPEND optionalLibs ${MATERIALX_LIBRARIES} hdMtlx)
    list(APPEND optionalIncludeDirs ${MATERIALX_INCLUDE_DIRS})
    list(APPEND optionalPublicClasses matfiltMaterialX)

    if (${PXR_ENABLE_OSL_SUPPORT})
        list(APPEND optionalLibs ${OSL_COMP_LIBRARY})
        list(APPEND optionalIncludeDirs ${OSL_INCLUDE_DIR})
    endif()
endif()

pxr_plugin(${PXR_PACKAGE}
    LIBRARIES
        ar
        arch
        plug
        tf
        vt
        gf
        hd
        hio
        usdRi
        usdVol
        usdVolImaging
        ${optionalLibs}

    INCLUDE_DIRS
        ${RENDERMAN_INCLUDE_DIR}
        ${optionalIncludeDirs}

    PUBLIC_CLASSES
        basisCurves
        camera
        context
        coordSys
        debugCodes
        gprim
        instancer
        light
        lightFilter
        lightFilterUtils
        material
        matfiltConvertPreviewMaterial
        matfiltFilterChain
        matfiltResolveVstructs
        mesh
        offlineContext
        points
        renderDelegate
        rixStrings
        volume
        xcpt
        ${optionalPublicClasses}

    PRIVATE_CLASSES
        interactiveContext
        interactiveRenderPass
        framebuffer
        offlineRenderPass
        renderBuffer
        rendererPlugin
        renderPass
        resourceRegistry

    PUBLIC_HEADERS
        api.h
        renderParam.h

    PRIVATE_HEADERS
        interactiveRenderParam.h
        virtualStructConditionalGrammar.tab.h

    CPPFILES
        virtualStructConditionalGrammar.tab.cpp
        virtualStructConditionalGrammar.lex.cpp

    RESOURCE_FILES
        plugInfo.json

    DISABLE_PRECOMPILED_HEADERS
)

# Include paths to hdPrman headers within hdPrman itself look like
# #include "hdPrman/...", so we need to add include/pxr/imaging
# to the include paths for this library. We use CMAKE_BINARY_DIR
# to include headers out of the build directory, which is what
# normally happens in the USD build.
if(TARGET hdPrman)
    target_include_directories(${PXR_PACKAGE}
        PRIVATE
            "${CMAKE_BINARY_DIR}/include/pxr/imaging"
    )
endif()

if(WIN32 AND NOT BUILD_SHARED_LIBS)
    # XXX: Temporarily disable this test on static Windows builds
    #      due to symbol visibility issues.
else()
    pxr_build_test(testHdPrman
        LIBRARIES
            cameraUtil
            hdPrman
            hd
            usd
            usdImaging
            usdRender
            ${PXRCORE_LIBRARY}
            ${PRMAN_LIBRARY}
        CPPFILES
            testenv/testHdPrman.cpp
    )
endif()
