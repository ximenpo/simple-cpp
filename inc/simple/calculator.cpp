
#include	<cmath>
#include	<cstdlib>
#include	<cstring>
#include	<sstream>

#include	"calculator.h"

static	const	double	PI	= 3.1415926535897932384626433832795;
static	const	double	E	= 2.71828182845904523536;

static double	Factorial(double x) {
    if(x < 0) throw "factorial can't handle negative values";
    double d = 1;
    for(int i=1; i<=(int)x; i++) d *= i;
    return d;
}

void calculator::reset() {
    err_msg_.clear();
    variables_.clear();
}

bool calculator::is_delimiter(char c) {
    return (c=='+' || c=='-' || c=='*' || c=='/' || c=='%' || c==',' || c==')' || c==0 );
}

void calculator::do_set_variable(const std::string& key, calculator::variable value) {
    variables_[key] = value;
}

calculator::variable calculator::do_find_variable(const char* buf, int& move) {
    move	= 0;
    for(variable_list::const_iterator it=variables_.begin(); it!=variables_.end(); it++) {
        if(memcmp( buf, it->first.c_str(), it->first.size()) == 0) {
            if(int(it->first.size()) > move) {
                move = int(it->first.size());
                return	it->second;
            }
        }
    }
    return move;
}

#define FX(FUNK,LEN,BIN,DO) if(!strncmp(buf,FUNK,LEN)) { move = LEN + fetch_func_params(buf+LEN,BIN,x,y); { DO; } }
calculator::variable calculator::do_find_function(const char* buf, int& move) {
    move	= 0;
    calculator::variable x,y;
    switch (*buf) {
    case 'a':
        FX("abs("  ,4,1, return std::abs(x););
        FX("asin(" ,5,1, if(!use_radian_) return ::asin(x)*360/(2*PI); return ::asin(x););
        FX("acos(" ,5,1, if(!use_radian_) return ::acos(x)*360/(2*PI); return ::acos(x););
        FX("atan(" ,5,1, if(!use_radian_) return ::atan(x)*360/(2*PI); return ::atan(x););
        break;
    case 'c':
        FX("cos("  ,4,1, if(!use_radian_) x = 2*PI/360*x; return ::cos(x););
        FX("cosh(" ,5,1, if(!use_radian_) x = 2*PI/360*x; return ::cosh(x););
        break;
    case 'e':
        FX("exp("  ,4,1, return ::exp(x););
        break;
    case 'i':
        // X:complex to double	FX("im("   ,3,0, return x.imag(););
        FX("int("  ,4,1, if(x<0) return ::ceil(x); return ::floor(x););
        break;
    case 'l':
        FX("ln("   ,3,1, return ::log(x););
        FX("log("  ,4,1, return ::log10(x););
        break;
    case 'm':
        FX("max("  ,4,2, if(x<y) return x; return y;);
        FX("min("  ,4,2, if(x>y) return x; return y;);
        break;
    case 'p':
        FX("pow("  ,4,2, return ::pow(x,y););
        break;
    case 's':
        FX("sqr("  ,4,1, return ::sqrt(x););
        FX("sin("  ,4,1, if(!use_radian_) x = 2*PI/360*x; return ::sin(x););
        FX("sqrn(" ,5,2, return ::pow(x,1/y);); // Bug p?negativa x v鋜den.
        FX("sinh(" ,5,1, if(!use_radian_) x = 2*PI/360*x; return ::sinh(x););
        break;
    case 't':
        FX("tan("  ,4,1, if(!use_radian_) x = 2*PI/360*x; return ::tan(x););
        FX("tanh(" ,5,1, if(!use_radian_) x = 2*PI/360*x; return ::tanh(x););
        break;
    case '!':
        FX("!("    ,2,1, return Factorial(x););
        break;
    case '(':
        FX("("     ,1,1, return x;)
        break;
    }
    return	move;
}
#undef	FX

bool calculator::execute(const char* str, variable& result) {
    if(NULL == str) {
        err_msg_ = "empty expression";
        return false;
    }

    std::string	buf;
    buf.reserve(1024 * 4);
    //
    // 删除空格
    //
    for(int i = 0; 0 != str[i]; ++i) {
        char c = str[i];
        if(!isspace(c)) {
            buf.push_back(c);
        }
    }
    if(buf.empty()) {
        // 空字符串
        err_msg_ = "empty expression";
        return false;
    }

    const char* pValue = strchr(buf.c_str(),'=');
    if(NULL == pValue) {
        // 表达式计算
        try {
            result	= this->calculate(buf.c_str(), int(buf.size()));
        } catch(const char* err) {
            err_msg_ = err;
            return false;
        } catch(...) {
            err_msg_ = "unknown exception";
            return false;
        }

        return true;
    }

    //
    // 变量赋值
    //
    const char* pKey = buf.c_str();
    if(!isalpha(*pKey) && '_' != *pKey) {
        err_msg_ = "variable must begin with alpha or _";
        return false;
    }
    for(const char* p = pKey; p < pValue; p++) {
        if( !isalnum(*p) && '_' != *p) {
            err_msg_ = "variable must be alphanumeric or _";
            return false;
        }
    }

    std::string key(pKey, pValue - pKey);
    if("e" == key || "PI" == key || "pi" == key ) {
        throw "variable shouldn't be reserved name(e/PI/pi)";
    }

    try {
        pValue++;
        result	= this->calculate(pValue, int(buf.size() - (pValue - pKey)));
    } catch(const char* err) {
        err_msg_ = err;
        return false;
    } catch(...) {
        err_msg_ = "unknown exception";
        return false;
    }

    this->do_set_variable(key,result);
    return true;
}

int calculator::fetch_func_params(const char *p, int arg_sum, calculator::variable &x, calculator::variable &y) {
    int size = 0, nr=1;
    while( nr ) {
        if(!p[size] ) throw "missing ')'";
        if( p[size] == '(') nr++;
        if( p[size] == ')') nr--;
        size++;
    }
    size--; // size 鋜  antal tecken mellan () tex: (aaa) -> size=3

    switch(arg_sum) {
    case 2: {
        int i=0;
        for(; i<size; i++) {
            if(p[i] == '(') {
                int nr2 = 1;
                while(nr2 && i < size) {
                    i++;
                    if(p[i] == '(') nr2++;
                    if(p[i] == ')') nr2--;
                }
            }
            if(p[i] == ',') break;
        }
        if(i == size) throw "missing ','";
        x = this->calculate(p, i);
        y = this->calculate(p + i + 1, size - i - 1);
    }
    break;
    case 1: {
        x = this->calculate(p, size);
    }
    break;
    case 0: {
    }
    break;
    default: {
        throw "invalid arg sum";
    }
    break;
    }
    return size + 1;
}

calculator::variable calculator::calculate(const char* p, int len) {
    int pos=0, move;
    bool neg = false;
    while(p[pos] == '+' || p[pos] == '-') {
        if(p[pos] == '-') neg =!neg;
        pos++;
    }
    calculator::variable d1 = this->search_number(p + pos, move);
    if(neg) d1 = -d1;
    pos += move;

    while(pos < len) {
        char c = p[pos];
        if(c == '+' || c == '-') return d1 + this->calculate( p + pos, len - pos );
        if(c == '*' || c == '/' || c == '%') {
            pos++;
            neg = false;
            while(p[pos] == '+' || p[pos] == '-') {
                if(p[pos] == '-') neg =!neg;
                pos++;
            }
            calculator::variable d2 = this->search_number(p + pos, move);
            if(neg) d2 = -d2;
            pos += move;
            if(c == '*') d1 *= d2;
            else if(d2 == calculator::variable(0)) throw "division by Zero";
            else if(c == '/') d1 /= d2;
            else d1 = ::fmod(d1,d2);
        } else {
            std::ostringstream os;
            os	<< "unexpected '" << p[pos] << "' at "	<< pos;
            throw os.str().c_str();
        }
    }
    return d1;
}

calculator::variable calculator::search_number(const char *p, int &move) {
    // function
    {
        move	= 0;
        variable ret = this->do_find_function(p, move);
        if(move > 0) return ret;
    }

    // variable
    {
        move	= 0;
        variable ret = this->do_find_variable(p, move);
        if(move > 0) return ret;
    }

    // hex number
    if(p[0] == '0' && (p[1] == 'x' || p[1] == 'X') ) {
        int i=0;
        char c;
        move = 1;
        {
            std::istringstream is(p);
            is >> std::hex >> i;
        }
        do {
            move++;
            c = ::tolower(p[move]);
        } while( (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'));
        return variable(i);
    }

    // binary number
    if(p[0] == '0' && (p[1] == 'b' || p[1] == 'B') ) {
        int i=0;
        move = 2;
        while(p[move] == '0' || p[move] == '1') {
            i *= 2;
            if(p[move] == '1') i++;
            move++;
        }
        return variable(i);
    }

    // dec number
    if( (*p>='0' && *p<='9') || *p=='.' ) {
        move=0;
        double d = ::atof(p);
        if   (p[move] == '+' || p[move] == '-') move++;
        while(p[move] >= '0' && p[move] <= '9') move++;
        if   (p[move] == '.') move++;
        while(p[move] >= '0' && p[move] <= '9') move++;
        if   (p[move] == 'e' || p[move] == 'E') {
            move++;
            if   (p[move] == '+' || p[move] == '-') move++;
            while(p[move] >= '0' && p[move] <= '9') move++;
        }
        return d;
    }

    // PI
    if(strncmp( p, "pi", 2) == 0 && is_delimiter(p[2]) ) {
        move = 2;
        return variable(PI);
    }

    // E
    if(p[0] == 'e' && is_delimiter(p[1]) ) {
        move = 1;
        return variable(E);
    }

    // error
    {
        int i=0;
        char tmp[100];
        while( !is_delimiter(p[i]) ) {
            tmp[i] = p[i];
            i++;
        }
        tmp[i]=0;
        if(i) {
            std::ostringstream os;
            os	<< "syntax error: '" << tmp << "'";
            throw os.str().c_str();
        }
        throw "syntax error";
    }
}
