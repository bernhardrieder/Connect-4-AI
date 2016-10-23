#pragma once

#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>
#include <limits>
#include <chrono>
#include <fstream>

#include "BoardSimulation.h"
#include "Negamax.h"
#include "Helper.h"
#include "Game.h"
#include "DiscHoles.h"
#include "Board.h"
#include "GlobalVariables.h"
#include "GameMode.h"
#include "GameModePvP.h"
#include "GameModePvAI.h"
#include "GameModeAIvAI.h"