#pragma once

#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "Texture.h"
#include "TexLed.h"
#include "Expr.h"


Texture andGate;
TexLed led1, led2, led3;
Expr expr;
