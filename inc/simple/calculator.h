#ifndef CALCULATOR_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define CALCULATOR_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include	<map>
#include	<string>

class calculator {
public:
    typedef	double	variable;
    typedef	std::map<std::string, variable>		variable_list;

public:
    calculator() {
        use_radian_ = true;
    }

public:
    //	��ձ�����������Ϣ
    void				reset();
    //	ִ����ѧ���ʽ����Ϊ��ֵ���򱣴�����������ͨ��get_variable��ȡ������ֱ��ͨ��result����
    bool				execute(const char* str, variable& result);
    //	���ñ���
    void				set_variable(const std::string& key, variable value) {
        this->do_set_variable(key, value);
    }
    //	��ȡ����ֵ
    bool				get_variable(const std::string& key, variable& value) {
        return	(this->do_find_variable(key.c_str(), value) > 0);
    }

    void				set_use_radian(bool use_radian) {
        use_radian_	= use_radian;
    }
    bool				use_radian() {
        return use_radian_;
    }

    const char*			error_message() {
        return err_msg_.c_str();
    }

protected:
    virtual	void		do_set_variable(const std::string& key, variable value);
    virtual	int			do_find_variable(const char* buf, variable& var);

    bool				is_delimiter(char c);

private:
    variable			calculate(const char* p, int len);
    variable			get_number(const char *p, int &move);
    int					funk(const char *p, bool binary, variable &x, variable &y);

private:
    bool				use_radian_;
    variable_list		variables_;
    std::string			err_msg_;
};

#endif
