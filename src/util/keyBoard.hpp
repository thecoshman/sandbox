#pragma once
#include <vector>
#include "common/events.hpp"

namespace util{
namespace {
    enum class KeyState{
        down, firstDown, firstUp, up
    };
}
struct KeyBoard{
    KeyBoard();
    bool isDown(Peanuts::KeyCode key);
    bool isFirstDown(Peanuts::KeyCode key);
    bool isUp(Peanuts::KeyCode key);
    bool isFirstUp(Peanuts::KeyCode key);
    void update(const Peanuts::Event::KeyDown& event);
    void update(const Peanuts::Event::KeyUp& event);
private:
    std::vector<KeyState> keyStates;
};
}