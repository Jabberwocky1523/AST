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
CMAKE_SOURCE_DIR = /home/kurisu/桌面/AST

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kurisu/桌面/AST/build

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

CMakeFiles/AST.dir/Test.o: CMakeFiles/AST.dir/flags.make
CMakeFiles/AST.dir/Test.o: /home/kurisu/桌面/AST/Test.cpp
CMakeFiles/AST.dir/Test.o: CMakeFiles/AST.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kurisu/桌面/AST/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/AST.dir/Test.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/AST.dir/Test.o -MF CMakeFiles/AST.dir/Test.o.d -o CMakeFiles/AST.dir/Test.o -c /home/kurisu/桌面/AST/Test.cpp

CMakeFiles/AST.dir/Test.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/AST.dir/Test.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kurisu/桌面/AST/Test.cpp > CMakeFiles/AST.dir/Test.i

CMakeFiles/AST.dir/Test.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/AST.dir/Test.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kurisu/桌面/AST/Test.cpp -o CMakeFiles/AST.dir/Test.s

CMakeFiles/AST.dir/ast.o: CMakeFiles/AST.dir/flags.make
CMakeFiles/AST.dir/ast.o: /home/kurisu/桌面/AST/ast.cpp
CMakeFiles/AST.dir/ast.o: CMakeFiles/AST.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kurisu/桌面/AST/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/AST.dir/ast.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/AST.dir/ast.o -MF CMakeFiles/AST.dir/ast.o.d -o CMakeFiles/AST.dir/ast.o -c /home/kurisu/桌面/AST/ast.cpp

CMakeFiles/AST.dir/ast.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/AST.dir/ast.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kurisu/桌面/AST/ast.cpp > CMakeFiles/AST.dir/ast.i

CMakeFiles/AST.dir/ast.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/AST.dir/ast.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kurisu/桌面/AST/ast.cpp -o CMakeFiles/AST.dir/ast.s

CMakeFiles/AST.dir/astMem.o: CMakeFiles/AST.dir/flags.make
CMakeFiles/AST.dir/astMem.o: /home/kurisu/桌面/AST/astMem.cpp
CMakeFiles/AST.dir/astMem.o: CMakeFiles/AST.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kurisu/桌面/AST/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/AST.dir/astMem.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/AST.dir/astMem.o -MF CMakeFiles/AST.dir/astMem.o.d -o CMakeFiles/AST.dir/astMem.o -c /home/kurisu/桌面/AST/astMem.cpp

CMakeFiles/AST.dir/astMem.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/AST.dir/astMem.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kurisu/桌面/AST/astMem.cpp > CMakeFiles/AST.dir/astMem.i

CMakeFiles/AST.dir/astMem.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/AST.dir/astMem.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kurisu/桌面/AST/astMem.cpp -o CMakeFiles/AST.dir/astMem.s

CMakeFiles/AST.dir/astObject.o: CMakeFiles/AST.dir/flags.make
CMakeFiles/AST.dir/astObject.o: /home/kurisu/桌面/AST/astObject.cpp
CMakeFiles/AST.dir/astObject.o: CMakeFiles/AST.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kurisu/桌面/AST/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/AST.dir/astObject.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/AST.dir/astObject.o -MF CMakeFiles/AST.dir/astObject.o.d -o CMakeFiles/AST.dir/astObject.o -c /home/kurisu/桌面/AST/astObject.cpp

CMakeFiles/AST.dir/astObject.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/AST.dir/astObject.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kurisu/桌面/AST/astObject.cpp > CMakeFiles/AST.dir/astObject.i

CMakeFiles/AST.dir/astObject.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/AST.dir/astObject.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kurisu/桌面/AST/astObject.cpp -o CMakeFiles/AST.dir/astObject.s

CMakeFiles/AST.dir/astState.o: CMakeFiles/AST.dir/flags.make
CMakeFiles/AST.dir/astState.o: /home/kurisu/桌面/AST/astState.cpp
CMakeFiles/AST.dir/astState.o: CMakeFiles/AST.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kurisu/桌面/AST/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/AST.dir/astState.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/AST.dir/astState.o -MF CMakeFiles/AST.dir/astState.o.d -o CMakeFiles/AST.dir/astState.o -c /home/kurisu/桌面/AST/astState.cpp

CMakeFiles/AST.dir/astState.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/AST.dir/astState.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kurisu/桌面/AST/astState.cpp > CMakeFiles/AST.dir/astState.i

CMakeFiles/AST.dir/astState.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/AST.dir/astState.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kurisu/桌面/AST/astState.cpp -o CMakeFiles/AST.dir/astState.s

CMakeFiles/AST.dir/astString.o: CMakeFiles/AST.dir/flags.make
CMakeFiles/AST.dir/astString.o: /home/kurisu/桌面/AST/astString.cpp
CMakeFiles/AST.dir/astString.o: CMakeFiles/AST.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kurisu/桌面/AST/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/AST.dir/astString.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/AST.dir/astString.o -MF CMakeFiles/AST.dir/astString.o.d -o CMakeFiles/AST.dir/astString.o -c /home/kurisu/桌面/AST/astString.cpp

CMakeFiles/AST.dir/astString.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/AST.dir/astString.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kurisu/桌面/AST/astString.cpp > CMakeFiles/AST.dir/astString.i

CMakeFiles/AST.dir/astString.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/AST.dir/astString.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kurisu/桌面/AST/astString.cpp -o CMakeFiles/AST.dir/astString.s

# Object files for target AST
AST_OBJECTS = \
"CMakeFiles/AST.dir/Test.o" \
"CMakeFiles/AST.dir/ast.o" \
"CMakeFiles/AST.dir/astMem.o" \
"CMakeFiles/AST.dir/astObject.o" \
"CMakeFiles/AST.dir/astState.o" \
"CMakeFiles/AST.dir/astString.o"

# External object files for target AST
AST_EXTERNAL_OBJECTS =

AST: CMakeFiles/AST.dir/Test.o
AST: CMakeFiles/AST.dir/ast.o
AST: CMakeFiles/AST.dir/astMem.o
AST: CMakeFiles/AST.dir/astObject.o
AST: CMakeFiles/AST.dir/astState.o
AST: CMakeFiles/AST.dir/astString.o
AST: CMakeFiles/AST.dir/build.make
AST: CMakeFiles/AST.dir/compiler_depend.ts
AST: CMakeFiles/AST.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/kurisu/桌面/AST/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable AST"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/AST.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/AST.dir/build: AST
.PHONY : CMakeFiles/AST.dir/build

CMakeFiles/AST.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/AST.dir/cmake_clean.cmake
.PHONY : CMakeFiles/AST.dir/clean

CMakeFiles/AST.dir/depend:
	cd /home/kurisu/桌面/AST/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kurisu/桌面/AST /home/kurisu/桌面/AST /home/kurisu/桌面/AST/build /home/kurisu/桌面/AST/build /home/kurisu/桌面/AST/build/CMakeFiles/AST.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/AST.dir/depend

