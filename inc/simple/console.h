#ifndef CONSOLE_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define CONSOLE_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include <string>
#include <iostream>

//
//	控制台辅助类
//
class console {
public:
    enum COLOR {
        BLACK	= 0,
        RED		= 1,
        GREEN	= 2,
        YELLOW	= 3,
        BLUE	= 4,
        MAGENTA	= 5,
        CYAN	= 6,
        WHITE	= 7,
    };

public:
    // create the console.
    static	bool			create(const char* szCaption, bool bNoCloseButton = false);
    // disable the [x] button of the console
    static	void			disable_close_button();
    // close the console and delete it
    static	void			destroy();

public:
    static	void			clear();//	Clears the console to the background color

    //	Print a value
    template<typename ValueType>
    static	inline	void	write(ValueType& v) {
        std::cout << v;
    }

    //	Print a char array and moves the cursor to the next line
    static	void			write_line();
    static	void			write_line(const std::string& str);
    static	void			write_line(const char* str);

    //	Read user input
    static	char			read();
    static	std::string		read_line();

    //	Set the console color
    static	void			set_bgcolor(COLOR color);
    static	void			set_fgcolor(COLOR color);
    static	void			set_highlight(bool highlight);
    static	void			restore_default_color();

    // set and get title of console window
    static	void			set_title(const std::string& str);
    static	void			set_title(const char* str);
    static	std::string		title();
};

#endif
