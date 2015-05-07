
#include	<string>
#include	<istream>
#include	"csv.h"

static	bool	ignore_crlf(std::istream& is) {
    while(!is.eof()) {
        switch(is.peek()) {
        case '\r':
        case '\n':
            is.get();
            break;
        default:
            return	true;
        }
    }
    return	true;
}

static	bool	read_field_normal(std::istream& is, std::string& field) {
    while(!is.eof()) {
        char	c	= is.peek();
        if(is.eof()) {
            return	true;
        }
        switch(c) {
        case '\r':
        case '\n':
            return	ignore_crlf(is);
        case ',':
            is.get();
            field.push_back(',');
            return	false;
        default:
            is.get();
            field.push_back(c);
        }
    }
    return	true;
}

static	bool	read_field_quoted(std::istream& is, std::string& field) {
    std::string	str;
    while(is.peek() == '"') {
        is.get();
        field.push_back('"');

        str.clear();
        std::getline(is, str, char('"'));
        field	+= str;
        field.push_back('"');

        switch(is.peek()) {
        //	internal "
        case '"':
            break;
        //	line end.
        case '\r':
        case '\n':
            return	ignore_crlf(is);
            break;
        //	field end.
        case ',':
            is.get();
            field.push_back(',');
            return	false;
            break;
        default:
            return	read_field_normal(is, field);
            break;
        }

        if(is.eof()) {
            break;
        }
    }
    return	true;
}

static	bool	read_field(std::istream& is, std::string& field) {
    if(is.peek() == '"') {
        return	read_field_quoted(is, field);
    } else {
        return	read_field_normal(is, field);
    }
}

std::istream& operator >>(std::istream& is, csv_line& line) {
    line.clear();
    while(!is.eof()) {
        if(read_field(is, line.m_sData)) {
            break;
        }
    }
    return	is;
}

void	csv_line::clear() {
    m_sData.clear();
    m_nPos	= 0;
}

bool	csv_line::empty() {
    return	m_sData.empty();
}
