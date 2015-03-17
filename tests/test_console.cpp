#include <igloo/igloo.h>
using namespace igloo;

#include "simple/console.h"

Context(console_context) {
    Spec(basic_usage) {
        console::set_bgcolor(console::BLUE);
        console::set_fgcolor(console::RED);

        console::write("Red/Blue|");

        console::set_highlight(true);
        console::write("Red/BlueH|");

        console::set_highlight(false);
        console::write("Red/Blue|");

        console::set_bgcolor(console::BLACK);
        console::set_fgcolor(console::WHITE);

        console::restore_default_color();
    }

    Spec(set_title_usage) {
        console::set_title("Test Caption");

        console::restore_default_color();
    }
};
