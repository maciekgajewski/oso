#pragma once

#include <SDL.h>

#include <memory>
#include <stdexcept>
#include <string>

namespace sdl {

class exception : public std::exception {
public:
    exception(const std::string& e)
        : _e(e)
    {
    }

    const char* what() const noexcept override { return _e.c_str(); }

private:
    std::string _e;
};

inline void throw_error[[noreturn]](const std::string& why)
{
    throw exception(why + " : " + ::SDL_GetError());
}
/*
template <typename T>
class uique_ptr_with_deleter_fun {
public:
    using deleter_fun_t = void (*)(T*);

    uique_ptr_with_deleter_fun() = default;
    uique_ptr_with_deleter_fun(T* p, deleter_fun_t d)
        : _p(p)
        , _d(d)
    {
    }
    uique_ptr_with_deleter_fun(const uique_ptr_with_deleter_fun&) = delete;
    uique_ptr_with_deleter_fun(uique_ptr_with_deleter_fun&& o) noexcept
    {
        _p = o._p;
        _d = o._d;
        o._p = nullptr;
    }

    ~uique_ptr_with_deleter_fun() { free(); }

    uique_ptr_with_deleter_fun& operator==(uique_ptr_with_deleter_fun&& o) noexcept
    {
        free();
        _p = o._p;
        _d = o._d;
        o._p = nullptr;
    }

private:
    void free()
    {
        if (_p)
            _d(_p);
    }

    T* _p = nullptr;
    deleter_fun_t _d = nullptr;
};
*/
//using win_ptr = std::unique_ptr<::SDL_Window, decltype([](::SDL_Window* w) { ::SDL_DestroyWindow(w); })>;
//struct WinDeleter {
//    WinDeleter() = default;
//    void operator()(::SDL_Window* win) { ::SDL_DestroyWindow(win); }
//};

template <typename T, void(DeleterFun)(T*)>
struct Deleter {
    Deleter() = default;
    void operator()(T* p) { DeleterFun(p); }
};

using win_ptr = std::unique_ptr<::SDL_Window, Deleter<::SDL_Window, &::SDL_DestroyWindow>>;
//using win_ptr = std::unique_ptr<::SDL_Window>;

inline win_ptr create_window(const char* title,
    int x,
    int y,
    int w,
    int h,
    Uint32 flags)
{
    ::SDL_Window* win = ::SDL_CreateWindow(title, x, y, w, h, flags);
    if (!win)
        throw_error("Failed to create window");

    return win_ptr(win);
}
}
