#pragma once

#define PROJECT_ROOT C:\dev\led-matrix\sw

#define TO_STRING(s) #s
#define ABSOLUTE_PATH(root, relative_path) TO_STRING(root\relative_path)
#define FROM_ROOT(library) ABSOLUTE_PATH(PROJECT_ROOT, library)
