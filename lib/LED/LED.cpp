#include "LED.hpp"

void LED::init()
{
    _red.set();
    _green.set();
    _blue.set();
}

void LED::red()
{
    _red.clear();
    _green.set();
    _blue.set();
}

void LED::green()
{
    _red.set();
    _green.clear();
    _blue.set();
}

void LED::blue()
{
    _red.set();
    _green.set();
    _blue.clear();
}

void LED::white()
{
    _red.clear();
    _green.clear();
    _blue.clear();
}

void LED::black()
{
    _red.set();
    _green.set();
    _blue.set();
}
