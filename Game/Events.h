#pragma once

namespace game {

    struct Event 
    {
        inline static const std::string PelletEaten = "PelletEaten"; 
        inline static const std::string ScoreChanged = "ScoreChanged"; 
        inline static const std::string LivesChanged = "LivesChanged"; 
    };
}