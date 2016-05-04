
#include	<cmath>
#include    <cfloat>

#include    "string.h"
#include    "turing_calculator.h"

turing_calculator::turing_calculator():machine_(0), instructions_(0), instructions_sum_(0), yield_execution_(false) {
}

turing_calculator::~turing_calculator() {
    delete[]    instructions_;
}

size_t   turing_calculator::set_str_variable(const std::string& key, const std::string& value) {
    variable    str_index;
    if(fetch_variable(key, str_index)) {
        str_variables_[size_t(str_index)]   = value;
        return  size_t(str_index);
    }

    size_t  index   = str_variables_.size();
    str_variables_.push_back(value);
    this->set_variable(key, variable(index));
    return  index;
}

const char* turing_calculator::get_str_variable(const std::string& key) {
    variable    index;
    if(!fetch_variable(key, index)) {
        return  0;
    }
    size_t  str_index   = size_t(index);
    return  get_str_variable(str_index);
}

const char* turing_calculator::get_str_variable(size_t str_var_index) {
    if(str_var_index >= str_variables_.size()) {
        return  0;
    }
    return  str_variables_[str_var_index].c_str();
}

int turing_calculator::set_label_variable(const std::string& key, int value) {
    int var;
    if(fetch_label_variable(key, var)) {
        return  var;
    }

    set_variable(key, variable(value));
    return  value;
}

bool turing_calculator::fetch_label_variable(const std::string& key, int& value) {
    variable    var;
    if(!fetch_variable(key, var)) {
        return  false;
    }
    value   = int(var);
    return  true;
}

bool    turing_calculator::execute_instruction(turing_machine* machine, int instruction_address) {
    if(instruction_address < 0 || instruction_address >= int(instructions_sum_)) {
        this->set_error_message("invalid instruction address");
        machine->stop();
        return  false;
    }

    this->set_error_message("");
    const char* pstr    = instructions_[instruction_address];
    switch(*pstr) {
    case 0:
    case '#':
    case ':':
    case '@': {
        return  true;
    }
    }

    machine_            = machine;
    yield_execution_    = false;

    variable    result;
    bool        succeed  = execute(pstr, result);

    machine_    = 0;
    return  succeed && !yield_execution_;
}

void    turing_calculator::do_reset() {
    delete[]    instructions_;
    instructions_       = 0;
    instructions_sum_   = 0;
    str_variables_.clear();

    calculator::do_reset();
}

#define FX(FUNK,LEN,BIN,DO) if(!strncmp(buf,FUNK,LEN)) { move = LEN + fetch_func_params(buf+LEN,BIN,x,y); { DO; } }
calculator::variable	turing_calculator::do_find_function(const char* buf, int& move) {
    calculator::variable x,y;
    switch (*buf) {
    case 'A':
        FX("AND(",      4,2, return ((std::abs(x) <= DBL_EPSILON) && (std::abs(y) <= DBL_EPSILON))?1.0:0.0;);
        break;
    case 'E':
        FX("EQ(",       3,2, return (std::abs(x-y) <= DBL_EPSILON)?1.0:0.0;);
        break;
    case 'G':
        FX("GOTO("  ,   5,1, if(machine_)machine_->set_instruction_address(int(x)); return variable(1););
        FX("GOTO_IF(",  8,2, if(machine_ && y > 0.0)machine_->set_instruction_address(int(x)); return variable(y>0?1:0););
        FX("GT(",       3,2, return x>y?1.0:0.0;);
        break;
    case 'L':
        FX("LT(",       3,2, return x<y?1.0:0.0;);
        break;
    case 'N':
        FX("NOT(",      4,1, return x<=0.0?1.0:0.0;);
        break;
    case 'O':
        FX("OR(",       3,2, return ((std::abs(x) <= DBL_EPSILON) || (std::abs(y) <= DBL_EPSILON))?1.0:0.0;);
        break;
    case 'S':
        FX("STOP(",     5,0, if(machine_)machine_->stop(); return variable(1););
        break;
    case 'Y':
        FX("YIELD(",    6,0, this->yield_execution_ = true; return variable(1););
        break;
    }
    return	calculator::do_find_function(buf, move);
}
#undef	FX

bool    turing_calculator::do_preprocess_instructions() {
    for(size_t i = 0; i < instructions_sum_; ++i) {
        const char* pstr    = instructions_[i];
        if(NULL != pstr) {
            while(*pstr && isspace(*pstr)) {
                pstr++;
            }

            instructions_[i]    = pstr;// ignore white spaces
            switch(*pstr) {
            case '@': {//str variable define
                std::string name;
                name.reserve(32);
                do {
                    name.push_back(*pstr);
                    pstr++;
                } while(isalnum(*pstr) || *pstr == '_');
                const char* pValue = strchr(pstr,'=');
                if(NULL == pValue) {
                    this->set_str_variable(name, "");
                } else {
                    this->set_str_variable(name, ++pValue);
                }
            }
            break;
            case ':': {//label define
                std::string name;
                name.reserve(32);
                do {
                    name.push_back(*pstr);
                    pstr++;
                } while(isalnum(*pstr) || *pstr == '_');
                this->set_label_variable(name, int(i));
            }
            break;
            }
        }
    }
    return  true;
}
