#ifndef INPUT_H
#define INPUT_H
    
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <string.h>
    
    GLFWwindow* window;

    int KeyChar(const char* character) {
        if (!character) {
            return GLFW_KEY_UNKNOWN;
        }
        if (strlen(character) == 1) {
            char ch = character[0];
            if (ch >= 'A' && ch <= 'Z') {
                return GLFW_KEY_A + (ch - 'A');
            }
            if (ch >= 'a' && ch <= 'z') {
                return GLFW_KEY_A + (ch - 'a');
            }
            if (ch >= '0' && ch <= '9') {
                return GLFW_KEY_0 + (ch - '0');
            }
            switch(ch) {
                case ' ': return GLFW_KEY_SPACE;
                case '\'': return GLFW_KEY_APOSTROPHE;
                case ',': return GLFW_KEY_COMMA;
                //case '-': return GLFW_KEY_MINUS;
                case '.': return GLFW_KEY_PERIOD;
                case '/': return GLFW_KEY_SLASH;
                case ';': return GLFW_KEY_SEMICOLON;
                case '=': return GLFW_KEY_EQUAL;
                case '[': return GLFW_KEY_LEFT_BRACKET;
                case '\\': return GLFW_KEY_BACKSLASH;
                case ']': return GLFW_KEY_RIGHT_BRACKET;
                case '`': return GLFW_KEY_GRAVE_ACCENT;
                case '-': return 47;
                case '+': return 93;
                //case '-': return GLFW_KEY_KP_SUBTRACT;
                //case '+': return GLFW_KEY_KP_ADD;
                case '*': return GLFW_KEY_KP_MULTIPLY;
                default: return GLFW_KEY_UNKNOWN;
            }
        }
        // Keypad
            if (strcmp(character, "KpDecimal") == 0) return GLFW_KEY_KP_DECIMAL;
            if (strcmp(character, "KpDivide") == 0) return GLFW_KEY_KP_DIVIDE;
            if (strcmp(character, "KpMultiply") == 0) return GLFW_KEY_KP_MULTIPLY;
            if (strcmp(character, "KpSubtract") == 0) return GLFW_KEY_KP_SUBTRACT;
            if (strcmp(character, "KpAdd") == 0) return GLFW_KEY_KP_ADD;
            if (strcmp(character, "KpEnter") == 0) return GLFW_KEY_KP_ENTER;
            if (strcmp(character, "KpEqual") == 0) return GLFW_KEY_KP_EQUAL;
            if (strcmp(character, "Kp0") == 0) return GLFW_KEY_KP_0;
            if (strcmp(character, "Kp1") == 0) return GLFW_KEY_KP_1;
            if (strcmp(character, "Kp2") == 0) return GLFW_KEY_KP_2;
            if (strcmp(character, "Kp3") == 0) return GLFW_KEY_KP_3;
            if (strcmp(character, "Kp4") == 0) return GLFW_KEY_KP_4;
            if (strcmp(character, "Kp5") == 0) return GLFW_KEY_KP_5;
            if (strcmp(character, "Kp6") == 0) return GLFW_KEY_KP_6;
            if (strcmp(character, "Kp7") == 0) return GLFW_KEY_KP_7;
            if (strcmp(character, "Kp8") == 0) return GLFW_KEY_KP_8;
            if (strcmp(character, "Kp9") == 0) return GLFW_KEY_KP_9;
        // Alphanumeric keys
            if (strcmp(character, "Space") == 0) return GLFW_KEY_SPACE;
            if (strcmp(character, "Apostrophe") == 0) return GLFW_KEY_APOSTROPHE;
            if (strcmp(character, "Comma") == 0) return GLFW_KEY_COMMA;
            if (strcmp(character, "Add") == 0) return GLFW_KEY_KP_ADD;
            if (strcmp(character, "Minus") == 0) return GLFW_KEY_MINUS;
            if (strcmp(character, "Period") == 0) return GLFW_KEY_PERIOD;
            if (strcmp(character, "Slash") == 0) return GLFW_KEY_SLASH;
            if (strcmp(character, "Semicolon") == 0) return GLFW_KEY_SEMICOLON;
            if (strcmp(character, "Equal") == 0) return GLFW_KEY_EQUAL;
            if (strcmp(character, "LeftBracket") == 0) return GLFW_KEY_LEFT_BRACKET;
            if (strcmp(character, "Brackslash") == 0) return GLFW_KEY_BACKSLASH;
            if (strcmp(character, "RightBracket") == 0) return GLFW_KEY_RIGHT_BRACKET;
            if (strcmp(character, "Grave") == 0) return GLFW_KEY_GRAVE_ACCENT;
            if (strcmp(character, "Esc") == 0) return GLFW_KEY_ESCAPE;
            if (strcmp(character, "Enter") == 0) return GLFW_KEY_ENTER;
            if (strcmp(character, "Tab") == 0) return GLFW_KEY_TAB;
            if (strcmp(character, "Backspace") == 0) return GLFW_KEY_BACKSPACE;
            if (strcmp(character, "Insert") == 0) return GLFW_KEY_INSERT;
            if (strcmp(character, "Delete") == 0) return GLFW_KEY_DELETE;
            if (strcmp(character, "Right") == 0) return GLFW_KEY_RIGHT;
            if (strcmp(character, "Left") == 0) return GLFW_KEY_LEFT;
            if (strcmp(character, "Down") == 0) return GLFW_KEY_DOWN;
            if (strcmp(character, "Up") == 0) return GLFW_KEY_UP;
            if (strcmp(character, "PageUp") == 0) return GLFW_KEY_PAGE_UP;
            if (strcmp(character, "PageDown") == 0) return GLFW_KEY_PAGE_DOWN;
            if (strcmp(character, "Home") == 0) return GLFW_KEY_HOME;
            if (strcmp(character, "End") == 0) return GLFW_KEY_END;
            if (strcmp(character, "CapsLock") == 0) return GLFW_KEY_CAPS_LOCK;
            if (strcmp(character, "ScrollLock") == 0) return GLFW_KEY_SCROLL_LOCK;
            if (strcmp(character, "NumLock") == 0) return GLFW_KEY_NUM_LOCK;
            if (strcmp(character, "PrintScreen") == 0) return GLFW_KEY_PRINT_SCREEN;
            if (strcmp(character, "Pause") == 0) return GLFW_KEY_PAUSE;
        // Function keys
            if (strncmp(character, "F", 1) == 0 && strlen(character) > 1) {
                int functionKeyNumber = atoi(character + 1);
                if (functionKeyNumber >= 1 && functionKeyNumber <= 25) {
                    return GLFW_KEY_F1 + (functionKeyNumber - 1);
                }
            }
        // Modifier keys
            if (strcmp(character, "LeftShift") == 0) return GLFW_KEY_LEFT_SHIFT;
            if (strcmp(character, "RightShift") == 0) return GLFW_KEY_RIGHT_SHIFT;
            if (strcmp(character, "LeftControl") == 0) return GLFW_KEY_LEFT_CONTROL;
            if (strcmp(character, "RightControl") == 0) return GLFW_KEY_RIGHT_CONTROL;
            if (strcmp(character, "LeftAlt") == 0) return GLFW_KEY_LEFT_ALT;
            if (strcmp(character, "RightAlt") == 0) return GLFW_KEY_RIGHT_ALT;
            if (strcmp(character, "LeftSuper") == 0) return GLFW_KEY_LEFT_SUPER;
            if (strcmp(character, "RightSuper") == 0) return GLFW_KEY_RIGHT_SUPER;
        // Gamepad keys  "WorkInProgress"
            if (strcmp(character, "GamepadA") == 0) return GLFW_GAMEPAD_BUTTON_A;
            if (strcmp(character, "GamepadB") == 0) return GLFW_GAMEPAD_BUTTON_B;
            if (strcmp(character, "GamepadX") == 0) return GLFW_GAMEPAD_BUTTON_X;
            if (strcmp(character, "GamepadY") == 0) return GLFW_GAMEPAD_BUTTON_Y;
            if (strcmp(character, "GamepadLeftBumper") == 0) return GLFW_GAMEPAD_BUTTON_LEFT_BUMPER;
            if (strcmp(character, "GamepadRightBumper") == 0) return GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER;
            if (strcmp(character, "GamepadBack") == 0) return GLFW_GAMEPAD_BUTTON_BACK;
            if (strcmp(character, "GamepadStart") == 0) return GLFW_GAMEPAD_BUTTON_START;
            if (strcmp(character, "GamepadGuide") == 0) return GLFW_GAMEPAD_BUTTON_GUIDE;
            if (strcmp(character, "GamepadLeftThumb") == 0) return GLFW_GAMEPAD_BUTTON_LEFT_THUMB;
            if (strcmp(character, "GamepadRightThumb") == 0) return GLFW_GAMEPAD_BUTTON_RIGHT_THUMB;
            if (strcmp(character, "GamepadDpadUp") == 0) return GLFW_GAMEPAD_BUTTON_DPAD_UP;
            if (strcmp(character, "GamepadDpadRight") == 0) return GLFW_GAMEPAD_BUTTON_DPAD_RIGHT;
            if (strcmp(character, "GamepadDpadDown") == 0) return GLFW_GAMEPAD_BUTTON_DPAD_DOWN;
            if (strcmp(character, "GamepadDpadLeft") == 0) return GLFW_GAMEPAD_BUTTON_DPAD_LEFT;
            if (strcmp(character, "GamepadCross") == 0) return GLFW_GAMEPAD_BUTTON_CROSS; // Alternative label for A
            if (strcmp(character, "GamepadCircle") == 0) return GLFW_GAMEPAD_BUTTON_CIRCLE; // Alternative label for B
            if (strcmp(character, "GamepadSquare") == 0) return GLFW_GAMEPAD_BUTTON_SQUARE; // Alternative label for X
            if (strcmp(character, "GamepadTriangle") == 0) return GLFW_GAMEPAD_BUTTON_TRIANGLE; // Alternative label for Y
        // Menu key
            if (strcmp(character, "Menu") == 0) return GLFW_KEY_MENU;
        // If no match found
        return GLFW_KEY_UNKNOWN;
    }

    // KEYS
        int isKeyDown(const char* character) {
            int key = KeyChar(character);
            return glfwGetKey(window, key) == GLFW_PRESS;
        }

        int isKeyUp(const char* character) {
            int key = KeyChar(character);
            return glfwGetKey(window, key) == GLFW_RELEASE;
        }

        static int lastState[GLFW_KEY_LAST + 1] = {0};
        static int toggleState[GLFW_KEY_LAST + 1] = {0};

        int isKey(const char* character) {
            int key = KeyChar(character);
            if (key == GLFW_KEY_UNKNOWN || key > GLFW_KEY_LAST) {
                return false;
            }
            if (isKeyDown(character) && lastState[key] == GLFW_RELEASE) {
                toggleState[key] = !toggleState[key];
            }
            lastState[key] = glfwGetKey(window, key);
            return toggleState[key];
        }

        void isKeyReset(const char* character) {
            int key = KeyChar(character);
            if (key != GLFW_KEY_UNKNOWN && key <= GLFW_KEY_LAST) {
                toggleState[key] = 0;
            }
        }

        void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            const char* actionString = "";
            switch (action) {
                case GLFW_PRESS:
                    actionString = "PRESSED";
                    break;
                case GLFW_RELEASE:
                    actionString = "RELEASED";
                    break;
                case GLFW_REPEAT:
                    actionString = "REPEATED";
                    break;
                default:
                    actionString = "UNKNOWN ACTION";
                    break;
            }
            const char* modString = "";
            if (mods == GLFW_MOD_SHIFT) {
                modString = "SHIFT";
            } else if (mods == GLFW_MOD_CONTROL) {
                modString = "CONTROL";
            } else if (mods == GLFW_MOD_ALT) {
                modString = "ALT";
            } else if (mods == GLFW_MOD_SUPER) {
                modString = "SUPER";
            } else if (mods == 0) {
                modString = "NONE";
            } else {
                modString = "COMBINED MODS";
            }
            printf("Key %d [%s] with scancode %d, action %s, and mods %s\n", key, glfwGetKeyName(key, scancode), scancode, actionString, modString);
        }


    // MOUSE

        typedef struct {
            double x, y;
        } Mouse;

        Mouse GetMousePos() {
            Mouse mouse = {0,0};
            glfwGetCursorPos(window, &mouse.x, &mouse.y);
            return (Mouse){mouse.x, mouse.y};
        }

        static int mouseLastState[GLFW_MOUSE_BUTTON_LAST + 1] = {0};
        static int mouseToggleState[GLFW_MOUSE_BUTTON_LAST + 1] = {0};

        int isMouseButtonDown(int button) {
            return glfwGetMouseButton(window, button) == GLFW_PRESS;
        }

        int isMouseButtonUp(int button) {
            return glfwGetMouseButton(window, button) == GLFW_RELEASE;
        }

        int isMouseButton(const int button) {
            int currentState = glfwGetMouseButton(window, button);
            if (currentState == GLFW_PRESS && mouseLastState[button] == GLFW_RELEASE) {
                mouseToggleState[button] = !mouseToggleState[button];
            }
            mouseLastState[button] = currentState;
            return mouseToggleState[button];
        }

        void isMouseButtonReset(const int button) {
            if (button <= GLFW_MOUSE_BUTTON_LAST) {
                mouseToggleState[button] = 0;
            }
        }

        typedef struct {
            double x, y;
        } MouseScroll;

        static MouseScroll currentScroll = {0.0, 0.0};

        MouseScroll GetScroll() {
            return currentScroll;
        }

        void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
            currentScroll.x += xoffset;
            currentScroll.y += yoffset;
        }

#endif // INPUT_H