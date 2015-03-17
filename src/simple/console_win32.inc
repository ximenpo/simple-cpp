
#include <conio.h>
#include <windows.h>

#if	defined(_MSC_VER)
#	pragma	comment(lib, "user32.lib")
#endif

extern "C"	HWND	WINBASEAPI	APIENTRY	GetConsoleWindow();

struct	ConsoleHandler {
    ConsoleHandler() : saved_(false) {
        memset(&console_info_, 0, sizeof(console_info_));
    }
    ~ConsoleHandler() {
        restore_default_color();
    }
public:
    void	save_default_color() {
        if(!saved_) {
            if(GetConsoleScreenBufferInfo(::GetStdHandle(STD_OUTPUT_HANDLE), &console_info_)) {
                saved_	= true;
            }
        }
    }
    void	restore_default_color() {
        if(saved_) {
            SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE),	console_info_.wAttributes);
        }
    }
private:
    bool	saved_;
    CONSOLE_SCREEN_BUFFER_INFO	console_info_;
};
static	ConsoleHandler	s_console_handler__;

bool	console::create(const char* szCaption, bool bNoCloseButton) {
    if(!AllocConsole()) {
        return	false;
    }

    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);

    if(bNoCloseButton) {
        disable_close_button();
    }

    // set the console title
    if(szCaption != NULL) {
        SetConsoleTitle(szCaption);
    }

    return	true;
}

void	console::disable_close_button() {
    HWND	hWnd	= GetConsoleWindow ();
    if(NULL != hWnd) {
        ::EnableMenuItem(::GetSystemMenu (hWnd, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_GRAYED );
        ::RemoveMenu	(::GetSystemMenu (hWnd, FALSE), SC_CLOSE, MF_BYCOMMAND);
    }
}

void	console::destroy() {
    FreeConsole();
}

void	console::clear() {
    system("cls");
}

static	bool&	Highlight_() {
    static bool value = false;
    return value;
}

static	int&	BGColor_() {
    static int value = 0;
    return value;
}
static	int&	FGColor_() {
    static int value = FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED;
    return value;
}

static	void	set_console_color() {
    s_console_handler__.save_default_color();
    SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE),	FGColor_() | (Highlight_() ?FOREGROUND_INTENSITY :0) | BGColor_());
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
#if	defined(_MSC_VER)
    return _getch();
#	else
    return getch();
#endif
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
    short val;
    switch(color) {
    default:
    case BLACK:
        val = 0;
        break;
    case BLUE:
        val = BACKGROUND_BLUE;
        break;
    case CYAN:
        val = BACKGROUND_BLUE	|BACKGROUND_GREEN;
        break;
    case GREEN:
        val = BACKGROUND_GREEN;
        break;
    case MAGENTA:
        val = BACKGROUND_BLUE	|BACKGROUND_RED;
        break;
    case RED:
        val = BACKGROUND_RED;
        break;
    case WHITE:
        val = BACKGROUND_BLUE	|BACKGROUND_GREEN	|BACKGROUND_RED;
        break;
    case YELLOW:
        val = BACKGROUND_GREEN	|BACKGROUND_RED;
        break;
    }
    BGColor_() = val;

    set_console_color();
}

void	console::set_fgcolor(console::COLOR color) {
    int val;
    switch(color) {
    default:
    case BLACK:
        val = 0;
        break;
    case BLUE:
        val = FOREGROUND_BLUE;
        break;
    case CYAN:
        val = FOREGROUND_BLUE	|FOREGROUND_GREEN;
        break;
    case GREEN:
        val = FOREGROUND_GREEN;
        break;
    case MAGENTA:
        val = FOREGROUND_BLUE	|FOREGROUND_RED;
        break;
    case RED:
        val = FOREGROUND_RED;
        break;
    case WHITE:
        val = FOREGROUND_BLUE	|FOREGROUND_GREEN	|FOREGROUND_RED;
        break;
    case YELLOW:
        val = FOREGROUND_GREEN	|FOREGROUND_RED;
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
    s_console_handler__.restore_default_color();
}

void	console::set_title(const std::string& str) {
    SetConsoleTitle(str.c_str());
}

void	console::set_title(const char* str) {
    SetConsoleTitle(str);
}

std::string	console::get_title() {
    char szWindowTitle[MAX_PATH] = {0};
    GetConsoleTitle(szWindowTitle, sizeof(szWindowTitle));

    return std::string(szWindowTitle);
}
