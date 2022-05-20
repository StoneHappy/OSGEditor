########################## QT ######################################
find_package(Qt5 COMPONENTS Widgets OpenGL)

find_package(qtadvanceddocking CONFIG REQUIRED)

######################### OSG ######################################
find_package(OpenSceneGraph 3.6.0 REQUIRED osgDB osgGA osgUtil osgText osgViewer osgWidget osgUtil osgManipulator osgVolume osgTerrain osgSim osgShadow osgFX osgParticle osgAnimation)

######################### GDAL ######################################
find_package(GDAL REQUIRED)

####################### opengl #####################################
find_package(OpenGL REQUIRED)

add_subdirectory(Vendors)
add_subdirectory(Runtime)
add_subdirectory(App)