#include "keyBoard.hpp"

namespace util{
    KeyBoard::KeyBoard(): keyStates(static_cast<int>(Peanuts::KeyCode::UNKOWN_KEY), KeyState::firstUp){
        keyStates.shrink_to_fit();
    }

    bool KeyBoard::isDown(Peanuts::KeyCode key){
        if(keyStates[static_cast<int>(key)] == KeyState::firstDown || keyStates[static_cast<int>(key)] == KeyState::down){
            keyStates[static_cast<int>(key)] = KeyState::down;
            return true;
        }
        return false;
    }
    
    bool KeyBoard::isFirstDown(Peanuts::KeyCode key){
        if(keyStates[static_cast<int>(key)] == KeyState::firstDown){
            keyStates[static_cast<int>(key)] = KeyState::down;
            return true;
        }
        return false;
    }
    
    bool KeyBoard::isUp(Peanuts::KeyCode key){
        if(keyStates[static_cast<int>(key)] == KeyState::firstUp || keyStates[static_cast<int>(key)] == KeyState::up){
            keyStates[static_cast<int>(key)] = KeyState::up;
            return true;
        }
        return false;
    }
    
    bool KeyBoard::isFirstUp(Peanuts::KeyCode key){
        if(keyStates[static_cast<int>(key)] == KeyState::firstUp){
            keyStates[static_cast<int>(key)] = KeyState::up;
            return true;
        }
        return false;
    }
    
    void KeyBoard::update(const Peanuts::Event::KeyDown& event){
        if(event.key == Peanuts::KeyCode::P){ std::cout << "P down!\n"; }
        if(keyStates[static_cast<int>(event.key)] == KeyState::down){ return; }
        keyStates[static_cast<int>(event.key)] = KeyState::firstDown;
    }
    
    void KeyBoard::update(const Peanuts::Event::KeyUp& event){
        if(event.key == Peanuts::KeyCode::P){ std::cout << "P up!\n"; }
        if(keyStates[static_cast<int>(event.key)] == KeyState::up){ return; }
        keyStates[static_cast<int>(event.key)] = KeyState::firstUp;
    }
}