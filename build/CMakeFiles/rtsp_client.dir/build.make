# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mecasys/MyProjects/MJPEG_Viewer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mecasys/MyProjects/MJPEG_Viewer/build

# Include any dependencies generated for this target.
include CMakeFiles/rtsp_client.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/rtsp_client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/rtsp_client.dir/flags.make

CMakeFiles/rtsp_client.dir/src/client.cpp.o: CMakeFiles/rtsp_client.dir/flags.make
CMakeFiles/rtsp_client.dir/src/client.cpp.o: ../src/client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mecasys/MyProjects/MJPEG_Viewer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/rtsp_client.dir/src/client.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/rtsp_client.dir/src/client.cpp.o -c /home/mecasys/MyProjects/MJPEG_Viewer/src/client.cpp

CMakeFiles/rtsp_client.dir/src/client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rtsp_client.dir/src/client.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mecasys/MyProjects/MJPEG_Viewer/src/client.cpp > CMakeFiles/rtsp_client.dir/src/client.cpp.i

CMakeFiles/rtsp_client.dir/src/client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rtsp_client.dir/src/client.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mecasys/MyProjects/MJPEG_Viewer/src/client.cpp -o CMakeFiles/rtsp_client.dir/src/client.cpp.s

CMakeFiles/rtsp_client.dir/src/abs_detector.cpp.o: CMakeFiles/rtsp_client.dir/flags.make
CMakeFiles/rtsp_client.dir/src/abs_detector.cpp.o: ../src/abs_detector.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mecasys/MyProjects/MJPEG_Viewer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/rtsp_client.dir/src/abs_detector.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/rtsp_client.dir/src/abs_detector.cpp.o -c /home/mecasys/MyProjects/MJPEG_Viewer/src/abs_detector.cpp

CMakeFiles/rtsp_client.dir/src/abs_detector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rtsp_client.dir/src/abs_detector.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mecasys/MyProjects/MJPEG_Viewer/src/abs_detector.cpp > CMakeFiles/rtsp_client.dir/src/abs_detector.cpp.i

CMakeFiles/rtsp_client.dir/src/abs_detector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rtsp_client.dir/src/abs_detector.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mecasys/MyProjects/MJPEG_Viewer/src/abs_detector.cpp -o CMakeFiles/rtsp_client.dir/src/abs_detector.cpp.s

CMakeFiles/rtsp_client.dir/src/fake_detector.cpp.o: CMakeFiles/rtsp_client.dir/flags.make
CMakeFiles/rtsp_client.dir/src/fake_detector.cpp.o: ../src/fake_detector.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mecasys/MyProjects/MJPEG_Viewer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/rtsp_client.dir/src/fake_detector.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/rtsp_client.dir/src/fake_detector.cpp.o -c /home/mecasys/MyProjects/MJPEG_Viewer/src/fake_detector.cpp

CMakeFiles/rtsp_client.dir/src/fake_detector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rtsp_client.dir/src/fake_detector.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mecasys/MyProjects/MJPEG_Viewer/src/fake_detector.cpp > CMakeFiles/rtsp_client.dir/src/fake_detector.cpp.i

CMakeFiles/rtsp_client.dir/src/fake_detector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rtsp_client.dir/src/fake_detector.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mecasys/MyProjects/MJPEG_Viewer/src/fake_detector.cpp -o CMakeFiles/rtsp_client.dir/src/fake_detector.cpp.s

CMakeFiles/rtsp_client.dir/src/abs_detection_sender.cpp.o: CMakeFiles/rtsp_client.dir/flags.make
CMakeFiles/rtsp_client.dir/src/abs_detection_sender.cpp.o: ../src/abs_detection_sender.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mecasys/MyProjects/MJPEG_Viewer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/rtsp_client.dir/src/abs_detection_sender.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/rtsp_client.dir/src/abs_detection_sender.cpp.o -c /home/mecasys/MyProjects/MJPEG_Viewer/src/abs_detection_sender.cpp

CMakeFiles/rtsp_client.dir/src/abs_detection_sender.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rtsp_client.dir/src/abs_detection_sender.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mecasys/MyProjects/MJPEG_Viewer/src/abs_detection_sender.cpp > CMakeFiles/rtsp_client.dir/src/abs_detection_sender.cpp.i

CMakeFiles/rtsp_client.dir/src/abs_detection_sender.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rtsp_client.dir/src/abs_detection_sender.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mecasys/MyProjects/MJPEG_Viewer/src/abs_detection_sender.cpp -o CMakeFiles/rtsp_client.dir/src/abs_detection_sender.cpp.s

CMakeFiles/rtsp_client.dir/src/utils.cpp.o: CMakeFiles/rtsp_client.dir/flags.make
CMakeFiles/rtsp_client.dir/src/utils.cpp.o: ../src/utils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mecasys/MyProjects/MJPEG_Viewer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/rtsp_client.dir/src/utils.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/rtsp_client.dir/src/utils.cpp.o -c /home/mecasys/MyProjects/MJPEG_Viewer/src/utils.cpp

CMakeFiles/rtsp_client.dir/src/utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rtsp_client.dir/src/utils.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mecasys/MyProjects/MJPEG_Viewer/src/utils.cpp > CMakeFiles/rtsp_client.dir/src/utils.cpp.i

CMakeFiles/rtsp_client.dir/src/utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rtsp_client.dir/src/utils.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mecasys/MyProjects/MJPEG_Viewer/src/utils.cpp -o CMakeFiles/rtsp_client.dir/src/utils.cpp.s

CMakeFiles/rtsp_client.dir/src/impl/unix.cc.o: CMakeFiles/rtsp_client.dir/flags.make
CMakeFiles/rtsp_client.dir/src/impl/unix.cc.o: ../src/impl/unix.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mecasys/MyProjects/MJPEG_Viewer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/rtsp_client.dir/src/impl/unix.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/rtsp_client.dir/src/impl/unix.cc.o -c /home/mecasys/MyProjects/MJPEG_Viewer/src/impl/unix.cc

CMakeFiles/rtsp_client.dir/src/impl/unix.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rtsp_client.dir/src/impl/unix.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mecasys/MyProjects/MJPEG_Viewer/src/impl/unix.cc > CMakeFiles/rtsp_client.dir/src/impl/unix.cc.i

CMakeFiles/rtsp_client.dir/src/impl/unix.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rtsp_client.dir/src/impl/unix.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mecasys/MyProjects/MJPEG_Viewer/src/impl/unix.cc -o CMakeFiles/rtsp_client.dir/src/impl/unix.cc.s

CMakeFiles/rtsp_client.dir/src/custom_detection_sender.cpp.o: CMakeFiles/rtsp_client.dir/flags.make
CMakeFiles/rtsp_client.dir/src/custom_detection_sender.cpp.o: ../src/custom_detection_sender.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mecasys/MyProjects/MJPEG_Viewer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/rtsp_client.dir/src/custom_detection_sender.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/rtsp_client.dir/src/custom_detection_sender.cpp.o -c /home/mecasys/MyProjects/MJPEG_Viewer/src/custom_detection_sender.cpp

CMakeFiles/rtsp_client.dir/src/custom_detection_sender.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rtsp_client.dir/src/custom_detection_sender.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mecasys/MyProjects/MJPEG_Viewer/src/custom_detection_sender.cpp > CMakeFiles/rtsp_client.dir/src/custom_detection_sender.cpp.i

CMakeFiles/rtsp_client.dir/src/custom_detection_sender.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rtsp_client.dir/src/custom_detection_sender.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mecasys/MyProjects/MJPEG_Viewer/src/custom_detection_sender.cpp -o CMakeFiles/rtsp_client.dir/src/custom_detection_sender.cpp.s

# Object files for target rtsp_client
rtsp_client_OBJECTS = \
"CMakeFiles/rtsp_client.dir/src/client.cpp.o" \
"CMakeFiles/rtsp_client.dir/src/abs_detector.cpp.o" \
"CMakeFiles/rtsp_client.dir/src/fake_detector.cpp.o" \
"CMakeFiles/rtsp_client.dir/src/abs_detection_sender.cpp.o" \
"CMakeFiles/rtsp_client.dir/src/utils.cpp.o" \
"CMakeFiles/rtsp_client.dir/src/impl/unix.cc.o" \
"CMakeFiles/rtsp_client.dir/src/custom_detection_sender.cpp.o"

# External object files for target rtsp_client
rtsp_client_EXTERNAL_OBJECTS =

../bin/rtsp_client: CMakeFiles/rtsp_client.dir/src/client.cpp.o
../bin/rtsp_client: CMakeFiles/rtsp_client.dir/src/abs_detector.cpp.o
../bin/rtsp_client: CMakeFiles/rtsp_client.dir/src/fake_detector.cpp.o
../bin/rtsp_client: CMakeFiles/rtsp_client.dir/src/abs_detection_sender.cpp.o
../bin/rtsp_client: CMakeFiles/rtsp_client.dir/src/utils.cpp.o
../bin/rtsp_client: CMakeFiles/rtsp_client.dir/src/impl/unix.cc.o
../bin/rtsp_client: CMakeFiles/rtsp_client.dir/src/custom_detection_sender.cpp.o
../bin/rtsp_client: CMakeFiles/rtsp_client.dir/build.make
../bin/rtsp_client: /usr/local/lib/libopencv_gapi.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_stitching.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_aruco.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_bgsegm.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_bioinspired.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_ccalib.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_dnn_objdetect.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_dnn_superres.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_dpm.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_face.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_freetype.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_fuzzy.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_hfs.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_img_hash.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_intensity_transform.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_line_descriptor.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_mcc.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_quality.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_rapid.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_reg.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_rgbd.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_saliency.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_stereo.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_structured_light.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_superres.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_surface_matching.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_tracking.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_videostab.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_wechat_qrcode.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_xfeatures2d.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_xobjdetect.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_xphoto.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_shape.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_highgui.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_datasets.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_plot.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_text.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_ml.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_phase_unwrapping.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_optflow.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_ximgproc.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_video.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_videoio.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_imgcodecs.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_objdetect.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_calib3d.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_dnn.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_features2d.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_flann.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_photo.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_imgproc.so.4.8.0
../bin/rtsp_client: /usr/local/lib/libopencv_core.so.4.8.0
../bin/rtsp_client: CMakeFiles/rtsp_client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mecasys/MyProjects/MJPEG_Viewer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable ../bin/rtsp_client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rtsp_client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/rtsp_client.dir/build: ../bin/rtsp_client

.PHONY : CMakeFiles/rtsp_client.dir/build

CMakeFiles/rtsp_client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/rtsp_client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/rtsp_client.dir/clean

CMakeFiles/rtsp_client.dir/depend:
	cd /home/mecasys/MyProjects/MJPEG_Viewer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mecasys/MyProjects/MJPEG_Viewer /home/mecasys/MyProjects/MJPEG_Viewer /home/mecasys/MyProjects/MJPEG_Viewer/build /home/mecasys/MyProjects/MJPEG_Viewer/build /home/mecasys/MyProjects/MJPEG_Viewer/build/CMakeFiles/rtsp_client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/rtsp_client.dir/depend

