#include "keymap.h"

std::string KeyMap::toString() const
{
    switch (type)
    {
    case DeviceType::KEYBOARD:   return toStringKeyboard();
    case DeviceType::JOYSTICK:   return toStringJoystick();
    case DeviceType::CONTROLLER: return toStringController();
    case DeviceType::MOUSE:      return toStringMouse();
    default: break;
    }
    return "";
}

void KeyMap::setKeyboard(Input::Keyboard kb)
{
    type = DeviceType::KEYBOARD;
    device = 0;
    keyboard = kb;
}

void KeyMap::setRawInput(int deviceID, int code)
{
    type = DeviceType::RAWINPUT;
    device = deviceID;
    this->code = code;
}

void KeyMap::fromString(const std::string_view& name)
{
    type = DeviceType::UNDEF;
    if (name.empty()) return;

    switch (name[0])
    {
    case 'K': fromStringKeyboard(name); break;
    case 'J': fromStringJoystick(name); break;
    case 'C': fromStringController(name); break;
    case 'M': fromStringMouse(name); break;
    default: break;
    }
}

void KeyMap::fromStringKeyboard(const std::string_view& name)
{
    if (name.substr(0, 2) != "K_") return;
    auto keystr = name.substr(2);
    for (size_t i = 0; i < 0xFF; ++i)
    {
        if (Input::keyboardNameMap[i] != NULL && keystr == Input::keyboardNameMap[i])
        {
            setKeyboard(static_cast<Input::Keyboard>(i));
            return;
        }
    }
    setKeyboard(Input::Keyboard::K_ERROR);
}

void KeyMap::fromStringJoystick(const std::string_view& name)
{
    if (name.substr(0, 2) != "J_") return;
    type = DeviceType::JOYSTICK;
    assert(false);
}

void KeyMap::fromStringController(const std::string_view& name)
{
    if (name.substr(0, 2) != "C_") return;
    type = DeviceType::CONTROLLER;
    assert(false);
}

void KeyMap::fromStringMouse(const std::string_view& name)
{
    if (name.substr(0, 2) != "M_") return;
    type = DeviceType::MOUSE;
    assert(false);
}

void KeyMap::fromStringRawInput(const std::string_view& name)
{
    if (name.substr(0, 2) != "R_") return;
    type = DeviceType::RAWINPUT;
    assert(false);
}

std::string KeyMap::toStringKeyboard() const
{
    return "K_"s + Input::keyboardNameMap[static_cast<size_t>(keyboard)];
}

std::string KeyMap::toStringJoystick() const
{
    assert(false);
    return "J_"s + "NULL";
}

std::string KeyMap::toStringController() const
{
    assert(false);
    return "C_"s + "NULL";
}

std::string KeyMap::toStringMouse() const
{
    assert(false);
    return "M_"s + "NULL";
}

std::string KeyMap::toStringRawInput() const
{
    assert(false);
    return "R_"s + "NULL";
}