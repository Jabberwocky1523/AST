# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kurisu/桌面/AST/test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kurisu/桌面/AST/test/build

# Include any dependencies generated for this target.
include CMakeFiles/AST.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/AST.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/AST.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/AST.dir/flags.make

CMakeFiles/AST.dir/codegen:
.PHONY : CMakeFiles/AST.dir/codegen

CMakeFiles/AST.dir/BinaryChunk.o: CMakeFiles/AST.dir/flags.make
CMakeFiles/AST.dir/BinaryChunk.o: /home/kurisu/桌面/AST/test/BinaryChunk.cpp
CMakeFiles/AST.dir/BinaryChunk.o: CMakeFiles/AST.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kurisu/桌面/AST/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/AST.dir/BinaryChunk.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/AST.dir/BinaryChunk.o -MF CMakeFiles/AST.dir/BinaryChunk.o.d -o CMakeFiles/AST.dir/BinaryChunk.o -c /home/kurisu/桌面/AST/test/BinaryChunk.cpp

CMakeFiles/AST.dir/BinaryChunk.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/AST.dir/BinaryChunk.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kurisu/桌面/AST/test/BinaryChunk.cpp > CMakeFiles/AST.dir/BinaryChunk.i

CMakeFiles/AST.dir/BinaryChunk.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/AST.dir/BinaryChunk.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kurisu/桌面/AST/test/BinaryChunk.cpp -o CMakeFiles/AST.dir/BinaryChunk.s

CMakeFiles/AST.dir/Buffer.o: CMakeFiles/AST.dir/flags.make
CMakeFiles/AST.dir/Buffer.o: /home/kurisu/桌面/AST/test/Buffer.cpp
CMakeFiles/AST.dir/Buffer.o: CMakeFiles/AST.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kurisu/桌面/AST/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/AST.dir/Buffer.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/AST.dir/Buffer.o -MF CMakeFiles/AST.dir/Buffer.o.d -o CMakeFiles/AST.dir/Buffer.o -c /home/kurisu/桌面/AST/test/Buffer.cpp

CMakeFiles/AST.dir/Buffer.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/AST.dir/Buffer.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kurisu/桌面/AST/test/Buffer.cpp > CMakeFiles/AST.dir/Buffer.i

CMakeFiles/AST.dir/Buffer.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/AST.dir/Buffer.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kurisu/桌面/AST/test/Buffer.cpp -o CMakeFiles/AST.dir/Buffer.s

CMakeFiles/AST.dir/BufferStream.o: CMakeFiles/AST.dir/flags.make
CMakeFiles/AST.dir/BufferStream.o: /home/kurisu/桌面/AST/test/BufferStream.cpp
CMakeFiles/AST.dir/BufferStream.o: CMakeFiles/AST.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kurisu/桌面/AST/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/AST.dir/BufferStream.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/AST.dir/BufferStream.o -MF CMakeFiles/AST.dir/BufferStream.o.d -o CMakeFiles/AST.dir/BufferStream.o -c /home/kurisu/桌面/AST/test/BufferStream.cpp

CMakeFiles/AST.dir/BufferStream.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/AST.dir/BufferStream.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kurisu/桌面/AST/test/BufferStream.cpp > CMakeFiles/AST.dir/BufferStream.i

CMakeFiles/AST.dir/BufferStream.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/AST.dir/BufferStream.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kurisu/桌面/AST/test/BufferStream.cpp -o CMakeFiles/AST.dir/BufferStream.s

CMakeFiles/AST.dir/log.o: CMakeFiles/AST.dir/flags.make
CMakeFiles/AST.dir/log.o: /home/kurisu/桌面/AST/test/log.cpp
CMakeFiles/AST.dir/log.o: CMakeFiles/AST.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kurisu/桌面/AST/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/AST.dir/log.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/AST.dir/log.o -MF CMakeFiles/AST.dir/log.o.d -o CMakeFiles/AST.dir/log.o -c /home/kurisu/桌面/AST/test/log.cpp

CMakeFiles/AST.dir/log.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/AST.dir/log.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kurisu/桌面/AST/test/log.cpp > CMakeFiles/AST.dir/log.i

CMakeFiles/AST.dir/log.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/AST.dir/log.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kurisu/桌面/AST/test/log.cpp -o CMakeFiles/AST.dir/log.s

CMakeFiles/AST.dir/main.o: CMakeFiles/AST.dir/flags.make
CMakeFiles/AST.dir/main.o: /home/kurisu/桌面/AST/test/main.cpp
CMakeFiles/AST.dir/main.o: CMakeFiles/AST.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kurisu/桌面/AST/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/AST.dir/main.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/AST.dir/main.o -MF CMakeFiles/AST.dir/main.o.d -o CMakeFiles/AST.dir/main.o -c /home/kurisu/桌面/AST/test/main.cpp

CMakeFiles/AST.dir/main.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/AST.dir/main.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kurisu/桌面/AST/test/main.cpp > CMakeFiles/AST.dir/main.i

CMakeFiles/AST.dir/main.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/AST.dir/main.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kurisu/桌面/AST/test/main.cpp -o CMakeFiles/AST.dir/main.s

CMakeFiles/AST.dir/utils.o: CMakeFiles/AST.dir/flags.make
CMakeFiles/AST.dir/utils.o: /home/kurisu/桌面/AST/test/utils.cpp
CMakeFiles/AST.dir/utils.o: CMakeFiles/AST.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kurisu/桌面/AST/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/AST.dir/utils.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/AST.dir/utils.o -MF CMakeFiles/AST.dir/utils.o.d -o CMakeFiles/AST.dir/utils.o -c /home/kurisu/桌面/AST/test/utils.cpp

CMakeFiles/AST.dir/utils.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/AST.dir/utils.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kurisu/桌面/AST/test/utils.cpp > CMakeFiles/AST.dir/utils.i

CMakeFiles/AST.dir/utils.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/AST.dir/utils.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kurisu/桌面/AST/test/utils.cpp -o CMakeFiles/AST.dir/utils.s

# Object files for target AST
AST_OBJECTS = \
"CMakeFiles/AST.dir/BinaryChunk.o" \
"CMakeFiles/AST.dir/Buffer.o" \
"CMakeFiles/AST.dir/BufferStream.o" \
"CMakeFiles/AST.dir/log.o" \
"CMakeFiles/AST.dir/main.o" \
"CMakeFiles/AST.dir/utils.o"

# External object files for target AST
AST_EXTERNAL_OBJECTS =

AST: CMakeFiles/AST.dir/BinaryChunk.o
AST: CMakeFiles/AST.dir/Buffer.o
AST: CMakeFiles/AST.dir/BufferStream.o
AST: CMakeFiles/AST.dir/log.o
AST: CMakeFiles/AST.dir/main.o
AST: CMakeFiles/AST.dir/utils.o
AST: CMakeFiles/AST.dir/build.make
AST: CMakeFiles/AST.dir/compiler_depend.ts
AST: CMakeFiles/AST.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/kurisu/桌面/AST/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable AST"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/AST.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/AST.dir/build: AST
.PHONY : CMakeFiles/AST.dir/build

CMakeFiles/AST.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/AST.dir/cmake_clean.cmake
.PHONY : CMakeFiles/AST.dir/clean

CMakeFiles/AST.dir/depend:
	cd /home/kurisu/桌面/AST/test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kurisu/桌面/AST/test /home/kurisu/桌面/AST/test /home/kurisu/桌面/AST/test/build /home/kurisu/桌面/AST/test/build /home/kurisu/桌面/AST/test/build/CMakeFiles/AST.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/AST.dir/depend

