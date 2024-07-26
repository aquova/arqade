#include "Utils.hpp"

bool AlphabetCompare(std::string aLhs, std::string aRhs) {
    return aLhs < aRhs;
}

std::string Join(std::string aLhs, std::string aRhs, std::string aDelim) {
    if (aLhs.empty()) {
        return aRhs;
    }
    return aLhs + aDelim + aRhs;
}

Qt::Key SDL2Qt(const SDL_Keycode aKey) {
    // Special shoutout to ChatGPT for writing this function
    // Finally, AI has come in handy
    switch (aKey) {
        case SDLK_UNKNOWN:        return Qt::Key_unknown;
        case SDLK_RETURN:         return Qt::Key_Return;
        case SDLK_ESCAPE:         return Qt::Key_Escape;
        case SDLK_BACKSPACE:      return Qt::Key_Backspace;
        case SDLK_TAB:            return Qt::Key_Tab;
        case SDLK_SPACE:          return Qt::Key_Space;
        case SDLK_EXCLAIM:        return Qt::Key_Exclam;
        case SDLK_QUOTEDBL:       return Qt::Key_QuoteDbl;
        case SDLK_HASH:           return Qt::Key_NumberSign;
        case SDLK_PERCENT:        return Qt::Key_Percent;
        case SDLK_DOLLAR:         return Qt::Key_Dollar;
        case SDLK_AMPERSAND:      return Qt::Key_Ampersand;
        case SDLK_QUOTE:          return Qt::Key_Apostrophe;
        case SDLK_LEFTPAREN:      return Qt::Key_ParenLeft;
        case SDLK_RIGHTPAREN:     return Qt::Key_ParenRight;
        case SDLK_ASTERISK:       return Qt::Key_Asterisk;
        case SDLK_PLUS:           return Qt::Key_Plus;
        case SDLK_COMMA:          return Qt::Key_Comma;
        case SDLK_MINUS:          return Qt::Key_Minus;
        case SDLK_PERIOD:         return Qt::Key_Period;
        case SDLK_SLASH:          return Qt::Key_Slash;
        case SDLK_0:              return Qt::Key_0;
        case SDLK_1:              return Qt::Key_1;
        case SDLK_2:              return Qt::Key_2;
        case SDLK_3:              return Qt::Key_3;
        case SDLK_4:              return Qt::Key_4;
        case SDLK_5:              return Qt::Key_5;
        case SDLK_6:              return Qt::Key_6;
        case SDLK_7:              return Qt::Key_7;
        case SDLK_8:              return Qt::Key_8;
        case SDLK_9:              return Qt::Key_9;
        case SDLK_COLON:          return Qt::Key_Colon;
        case SDLK_SEMICOLON:      return Qt::Key_Semicolon;
        case SDLK_LESS:           return Qt::Key_Less;
        case SDLK_EQUALS:         return Qt::Key_Equal;
        case SDLK_GREATER:        return Qt::Key_Greater;
        case SDLK_QUESTION:       return Qt::Key_Question;
        case SDLK_AT:             return Qt::Key_At;
        case SDLK_LEFTBRACKET:    return Qt::Key_BracketLeft;
        case SDLK_BACKSLASH:      return Qt::Key_Backslash;
        case SDLK_RIGHTBRACKET:   return Qt::Key_BracketRight;
        case SDLK_CARET:          return Qt::Key_AsciiCircum;
        case SDLK_UNDERSCORE:     return Qt::Key_Underscore;
        case SDLK_BACKQUOTE:      return Qt::Key_QuoteLeft;
        case SDLK_a:              return Qt::Key_A;
        case SDLK_b:              return Qt::Key_B;
        case SDLK_c:              return Qt::Key_C;
        case SDLK_d:              return Qt::Key_D;
        case SDLK_e:              return Qt::Key_E;
        case SDLK_f:              return Qt::Key_F;
        case SDLK_g:              return Qt::Key_G;
        case SDLK_h:              return Qt::Key_H;
        case SDLK_i:              return Qt::Key_I;
        case SDLK_j:              return Qt::Key_J;
        case SDLK_k:              return Qt::Key_K;
        case SDLK_l:              return Qt::Key_L;
        case SDLK_m:              return Qt::Key_M;
        case SDLK_n:              return Qt::Key_N;
        case SDLK_o:              return Qt::Key_O;
        case SDLK_p:              return Qt::Key_P;
        case SDLK_q:              return Qt::Key_Q;
        case SDLK_r:              return Qt::Key_R;
        case SDLK_s:              return Qt::Key_S;
        case SDLK_t:              return Qt::Key_T;
        case SDLK_u:              return Qt::Key_U;
        case SDLK_v:              return Qt::Key_V;
        case SDLK_w:              return Qt::Key_W;
        case SDLK_x:              return Qt::Key_X;
        case SDLK_y:              return Qt::Key_Y;
        case SDLK_z:              return Qt::Key_Z;
        case SDLK_DELETE:         return Qt::Key_Delete;
        case SDLK_KP_ENTER:       return Qt::Key_Enter;
        case SDLK_UP:             return Qt::Key_Up;
        case SDLK_DOWN:           return Qt::Key_Down;
        case SDLK_RIGHT:          return Qt::Key_Right;
        case SDLK_LEFT:           return Qt::Key_Left;
        case SDLK_INSERT:         return Qt::Key_Insert;
        case SDLK_HOME:           return Qt::Key_Home;
        case SDLK_END:            return Qt::Key_End;
        case SDLK_PAGEUP:         return Qt::Key_PageUp;
        case SDLK_PAGEDOWN:       return Qt::Key_PageDown;
        case SDLK_F1:             return Qt::Key_F1;
        case SDLK_F2:             return Qt::Key_F2;
        case SDLK_F3:             return Qt::Key_F3;
        case SDLK_F4:             return Qt::Key_F4;
        case SDLK_F5:             return Qt::Key_F5;
        case SDLK_F6:             return Qt::Key_F6;
        case SDLK_F7:             return Qt::Key_F7;
        case SDLK_F8:             return Qt::Key_F8;
        case SDLK_F9:             return Qt::Key_F9;
        case SDLK_F10:            return Qt::Key_F10;
        case SDLK_F11:            return Qt::Key_F11;
        case SDLK_F12:            return Qt::Key_F12;
        case SDLK_NUMLOCKCLEAR:   return Qt::Key_NumLock;
        case SDLK_CAPSLOCK:       return Qt::Key_CapsLock;
        case SDLK_SCROLLLOCK:     return Qt::Key_ScrollLock;
        case SDLK_RSHIFT:         return Qt::Key_Shift;
        case SDLK_LSHIFT:         return Qt::Key_Shift;
        case SDLK_RCTRL:          return Qt::Key_Control;
        case SDLK_LCTRL:          return Qt::Key_Control;
        case SDLK_RALT:           return Qt::Key_Alt;
        case SDLK_LALT:           return Qt::Key_Alt;
        case SDLK_MODE:           return Qt::Key_AltGr;
        case SDLK_HELP:           return Qt::Key_Help;
        case SDLK_PRINTSCREEN:    return Qt::Key_Print;
        case SDLK_SYSREQ:         return Qt::Key_SysReq;
        case SDLK_MENU:           return Qt::Key_Menu;
        case SDLK_POWER:          return Qt::Key_PowerOff;
        case SDLK_AUDIONEXT:      return Qt::Key_MediaNext;
        case SDLK_AUDIOPREV:      return Qt::Key_MediaPrevious;
        case SDLK_AUDIOSTOP:      return Qt::Key_MediaStop;
        case SDLK_AUDIOPLAY:      return Qt::Key_MediaPlay;
        case SDLK_VOLUMEUP:       return Qt::Key_VolumeUp;
        case SDLK_VOLUMEDOWN:     return Qt::Key_VolumeDown;
        default:                  return Qt::Key_unknown;
    }
}
