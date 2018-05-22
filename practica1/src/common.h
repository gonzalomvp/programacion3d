#pragma once

#include "../lib/glm/glm.hpp"
#include "../lib/glm/gtc/matrix_transform.hpp"
#include "../lib/glm/gtc/random.hpp"
#include "../lib/glm/gtc/type_ptr.hpp"
#include "../lib/glm/gtx/string_cast.hpp"
#include "vertex.h"
#include <fstream>
#include <GL/glew.h>
#include <iostream>
#include <memory>
#include <sstream> 
#include <stdint.h>

std::string readString(const std::string& filename);