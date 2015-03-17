
#include <cstdio>
#include <string>

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

struct	ConsoleHandler {
    ConsoleHandler()	{}
    ~ConsoleHandler() {
        console::restore_default_color();
    }
};
static	ConsoleHandler	s_console_handler__;

// create the console.
inline	bool	console::create(const char* szCaption, bool bNoCloseButton) {
    return	false;
}

// disable the [x] button of the console
inline	void	console::disable_close_button() {
}

// close the console and delete it
inline	void	console::destroy() {
}

static	bool&	Highlight_() {
    static bool value = false;
    return value;
}

static	std::string&	BGColor_() {
    static std::string value = "30";
    return value;
}
static	std::string&	FGColor_() {
    static std::string value = "47";
    return value;
}

static	void	set_console_color() {
    if(Highlight_()) {
        std::cout << "\033[1m";
    } else {
        std::cout << "\033[0m";
    }
    std::string clr = "\033[";
    clr += BGColor_();
    clr += ";";
    clr += FGColor_();
    clr += "m";
    std::cout << clr;
}

void	console::clear() {
    std::cout << "\033[2J";
}

void	console::write_line() {
    std::cout << std::endl;
}
void	console::write_line(const std::string& str) {
    std::cout << str << std::endl;
}
void	console::write_line(const char* str) {
    std::cout << str << std::endl;
}

char	console::read() {
    int ch;
    struct termios oldt,newt;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}

std::string	console::read_line() {
    std::string str;
    char c = getchar();
    while(c != '\n') {
        if(c == '\r')
            continue;
        str+=c;
        c = getchar();
    }
    return str;
}

void	console::set_bgcolor(console::COLOR color) {
    std::string val;
    switch(color) {
    default:
    case BLACK:
        val = "40";
        break;
    case BLUE:
        val = "44";
        break;
    case CYAN:
        val = "46";
        break;
    case GREEN:
        val = "42";
        break;
    case MAGENTA:
        val = "45";
        break;
    case RED:
        val = "41";
        break;
    case WHITE:
        val = "47";
        break;
    case YELLOW:
        val = "43";
        break;
    }
    BGColor_() = val;

    set_console_color();
}

void	console::set_fgcolor(console::COLOR color) {
    std::string val;
    switch(color) {
    case BLACK:
        val = "30";
        break;
    case BLUE:
        val = "34";
        break;
    case CYAN:
        val = "36";
        break;
    case GREEN:
        val = "32";
        break;
    case MAGENTA:
        val = "35";
        break;
    case RED:
        val = "31";
        break;
    default:
    case WHITE:
        val = "37";
        break;
    case YELLOW:
        val = "33";
        break;
    }
    FGColor_() = val;

    set_console_color();
}

void	console::set_highlight(bool highlight) {
    Highlight_()	= highlight;

    set_console_color();
}

void	console::restore_default_color() {
    std::cout << "\033[0m";
}

void	console::set_title(const std::string& str) {
    std::cout << "\033]0;\0" << str << "\x07\0";
}

void	console::set_title(const char* str) {
    std::cout << "\033]0;\0" << str << "\x07\0";
}

std::string	console::get_title() {
    return	std::string();
}
