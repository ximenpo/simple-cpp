#ifndef TURING_CALCULATOR_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define TURING_CALCULATOR_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include    <deque>
#include    <string>

#include    "calculator.h"
#include    "turing.h"

//
//  turing_calculator(added flow control and label&str defines)
//
//  [@str  = asdf ] define a str var named @str and value si ' asdf '
//  [:label]        define a lable var named @label for goto jump
//
//  GOTO(:label)            goto :label position directly
//  GOTO_IF(:label, cond)   goto :label position if cond > 0
//  STOP()                  stop the machine
//
//  GT(x,y)                 return 1 if x>y, 0 otherwise
//  LT(x,y)                 return 1 if x<y, 0 otherwise
//  NOT(x)                  return 1 if x<=0, 0 otherwise
//
class   turing_calculator : public calculator {
public:
    turing_calculator();
    ~turing_calculator();

public:
    //  load
    template<typename FwdIt>
    bool            load_instructions(FwdIt begin, size_t sum) {
        delete[]    this->instructions_;
        this->instructions_     = new const char*[sum];
        this->instructions_sum_ = sum;

        for(size_t i = 0; i < sum; ++i, ++begin) {
            this->instructions_[i]   = *begin;
        }

        return  this->do_preprocess_instructions();
    }

    //  str defines.
    size_t              set_str_variable(const std::string& key, const std::string& value);
    const char*         get_str_variable(const std::string& key);
    const char*         get_str_variable(size_t str_var_index);

    //  label defines.
    int                 set_label_variable(const std::string& key, int value);
    bool                fetch_label_variable(const std::string& key, int& value);

public:
    //  for machine binding
    bool                execute_instruction(turing_machine*, int instruction_address);

protected:
    virtual void        do_reset();
    virtual	variable	do_find_function(const char* buf, int& move);

    virtual bool        do_preprocess_instructions();

private:
    turing_machine*         machine_;
    const char**            instructions_;
    size_t                  instructions_sum_;
    std::deque<std::string> str_variables_;
};

#endif
