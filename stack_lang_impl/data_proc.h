#pragma once 
#ifndef  DATA_PROC_H
#define DATA_PROC_H

#include <stack>
#include <iostream>
#include <vector>
#include <any>
#include "../hash_set/hash_mset.h"
#include "functionSM.h"
using namespace std;

class processing {
    enum TYPE {
        TY_INT, 
        TY_HASH
    };

    // элемент стека 
    struct Var
    {
        bool type;
        int num;
        hash_set hash;
    };

    
    struct Variable 
    { 
        string name; // название переменной  
        TYPE type;  // тип переменной
        std::any val; // либо число, либо список 
    };

        
    vector<Variable>::iterator FindElem(std::vector<Variable>& t, string name)
    {
        vector<Variable>::iterator it;
        for (it = t.begin(); it != t.end(); ++it)
            if (it->name == name)
                return it;
        return it;
    }

    stack<Var> m_stack;
public:
    processing(vector<classToken> & vec_lex) {
        vector<Variable> t; 
        if (flag_Error == 1) {
            return;
        }

        for (vector<classToken>::iterator it = obj.begin(); it != obj.end(); ++it) {
            if (it->class_register == OPERATION)
            {
                if (m_stack.size() < 2)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nthere are not enough arguments to apply the OPERATOR " << (char)it->value <<  endl;
                    return;
                }
                Var v1, v2;
                Var res;
                res.type = 0;
                v1 = m_stack.top();
                m_stack.pop();
                v2 = m_stack.top();
                m_stack.pop();
                if (v1.type != TY_INT || v2.type != TY_INT)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nTHE OPERATION applies only to INT" << endl;
                    return;
                }
                if (it->value == '+')
                    res.num = v2.num + v1.num;
                else if (it->value == '-')
                    res.num = v2.num - v1.num;
                else if (it->value == '*')
                    res.num = v2.num * v1.num;
                else if (it->value == '/')
                    res.num = v2.num / v1.num;
                else if (it->value == '%')
                    res.num = v2.num % v1.num;
                m_stack.push(res);
            }
            else if (it->class_register == RATIO)
            {
                if (m_stack.size() < 2)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nthere are not enough arguments to apply the RATIO " << endl;
                    return;
                }

                if (m_stack.top().type == 0)
                {
                    Var res;
                    res.type = 0;
                    Var v1 = m_stack.top();
                    m_stack.pop();
                    Var v2 = m_stack.top();
                    m_stack.pop();
                    if (v1.type != TY_INT || v2.type != TY_INT)
                    {
                        cout << "RUNTIME ERROR. line: " << it->str << "\nTHE OPERATION applies only to INT" << endl;  
                        return;
                    }
                    if (it->value == LESS)
                        res.num = v2.num < v1.num;
                    else if (it->value == MORE)
                        res.num = v2.num > v1.num;
                    else if (it->value == LESS_EQUAL)
                        res.num = v2.num <= v1.num;
                    else if (it->value == MORE_EQUAL)
                        res.num = v2.num >= v1.num;
                    else if (it->value == EQUAL)
                        res.num = v2.num == v1.num;
                    else if (it->value == INEQUAL)
                        res.num = v2.num != v1.num;

                    m_stack.push(res);
                }
                else if (m_stack.top().type == 1)
                {
                    Var res;
                    res.type = 0;
                    Var v1 = m_stack.top();
                    m_stack.pop();
                    Var v2 = m_stack.top();
                    m_stack.pop();
                    if (v1.type != 1 || v2.type != 1)
                    {
                        cout << "RUNTIME ERROR. line: " << it->str << "\nTHE OPERATION applies only to HASH" << endl;
                        return;
                    }
                    if (it->value == EQUAL)
                        res.num = (v2.hash == v1.hash);
                    else if (it->value == INEQUAL)
                        res.num = (v2.hash != v1.hash);

                    m_stack.push(res);
                }
            }
            else if (it->class_register == READ)
            {
                Var k;
                k.type = 0;
                cout << "enter a number:";
                cin >> k.num;
                cin.get();
                m_stack.push(k);
            }
            else if (it->class_register == WRITE)
            {
                if (m_stack.size() < 1)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nthere are not enough arguments to apply the WRITE" << endl;
                    return;
                }
                if (m_stack.top().type == 0)
                    cout << m_stack.top().num << endl;
                else if (m_stack.top().type == 1)
                    cout << m_stack.top().hash << endl;

                m_stack.pop();
            }
            else if (it->class_register == POP) 
            {
                if (m_stack.size() < 1)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nthere are not enough arguments to apply the POP" << endl;
                    return;
                }
                if (m_stack.top().type == 0)
                {
                    auto iter = FindElem(t, it->name); 
                    if (iter == t.end())
                    {
                        Variable k;
                        k.type = TY_INT;
                        k.name = it->name;
                        k.val = m_stack.top().num;
                        m_stack.pop();
                        t.push_back(k);
                    }
                    else
                    {
                        iter->val = m_stack.top().num;
                        iter->type = TY_INT;
                        m_stack.pop();
                    }
                }
                else if (m_stack.top().type == 1)
                {
                    auto iter = FindElem(t, it->name);
                    if (iter == t.end())
                    {
                        Variable k;
                        k.type = TY_HASH;
                        k.name = it->name;
                        k.val = m_stack.top().hash;
                        m_stack.pop();
                        t.push_back(k);
                    }
                    else
                    {
                        iter->val = m_stack.top().hash;
                        iter->type = TY_HASH;
                        m_stack.pop();
                    }
                }
            }
            else if (it->class_register == PUSH)
            {

                if (it->type == 1)
                {
                    if (it->const_flag == 1)
                    {
                        Var k;
                        k.type = 1;
                        k.hash = it->hash;
                        m_stack.push(k);
                    }
                }
                else if (it->type == 0)
                {
                    if (it->const_flag == 1)
                    {
                        Var k;
                        k.type = 0;
                        k.num = it->value;
                        m_stack.push(k);
                    }
                }
                if (it->const_flag == 0)
                {
                    auto iter = FindElem(t, it->name);
                    if (iter == t.end())
                    {
                        cout << "Error in line: " << it->str << "\nthere is not such an argument" << endl;
                        return;
                    }
                    Var k;
                    k.type = iter->type;
                    if (k.type == 0)
                        k.num = std::any_cast<int>(iter->val);
                    else if (k.type == 1)
                        k.hash = std::any_cast<hash_set>(iter->val);
                    m_stack.push(k);
                }
            }
            else if (it->class_register == JI)
            {
                if (m_stack.top().num != 0)
                {
                    m_stack.pop();
                    int val = it->value;
                    it = obj.begin();
                    while (it->str != val)
                        ++it;
                    --it;
                }
            }
            else if (it->class_register == JMP)
            {
                int val = it->value;
                it = obj.begin();
                while (it->str != val)
                    ++it;
                --it;
            }
            else if (it->class_register == END)
                break;
            else if (it->class_register == LIREAD)
            {
                Var k;
                hash_set hash1;
                cout << "enter a multiset:";
                cin >> hash1;
                k.type = 1;
                k.hash = hash1;
                m_stack.push(k);
            }
            else if (it->class_register == ADD)
            {
                if (m_stack.size() < 1)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nthere are not enough arguments to apply the ADD" << endl;
                    return;
                }
                if (m_stack.top().type != 1)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nTHE OPERATION applies only to HASH" << endl;
                    return;
                }
                Var k;
                k.type = 1;
                hash_set obj = m_stack.top().hash;
                obj.add_the_first_missing();
                k.hash = obj;
                m_stack.pop();
                m_stack.push(k);
            }
            else if (it->class_register == DIFF)
            {
                if (m_stack.size() < 2)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nthere are not enough arguments to apply the DIFF" << endl;
                    return;
                }
                Var res;
                hash_set hash1, hash2;
                if (m_stack.top().type != 1)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nTHE OPERATION applies only to HASH" << endl;
                    return;
                }
                hash1 = m_stack.top().hash;
                m_stack.pop();
                if (m_stack.top().type != 1)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nTHE OPERATION applies only to HASH" << endl;
                    return;
                }
                hash2 = m_stack.top().hash;
                m_stack.pop();
                res.type = 1;
                res.hash = set_diff(hash2, hash1);
                m_stack.push(res);
            }
            else if (it->class_register == SYMM)
            {
                if (m_stack.size() < 2)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nthere are not enough arguments to apply the SYMM" << endl;
                    return;
                }
                Var res;
                hash_set hash1, hash2;
                if (m_stack.top().type != 1)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nTHE OPERATION applies only to HASH" << endl;
                    return;
                }
                hash1 = m_stack.top().hash;
                m_stack.pop();
                if (m_stack.top().type != 1)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nTHE OPERATION applies only to HASH" << endl;
                    return;
                }
                hash2 = m_stack.top().hash;
                m_stack.pop();
                res.type = 1;
                res.hash = set_symm_diff(hash2, hash1); 
                m_stack.push(res);
            }
            else if (it->class_register == UNION)
            {
                if (m_stack.size() < 2)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nthere are not enough arguments to apply the UNION" << endl;
                    return;
                }
                Var res;
                hash_set hash1, hash2;
                if (m_stack.top().type != 1)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nTHE OPERATION applies only to HASH" << endl;
                    return;
                }
                hash1 = m_stack.top().hash;
                m_stack.pop();
                if (m_stack.top().type != 1)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nTHE OPERATION applies only to HASH" << endl;
                    return;
                }
                hash2 = m_stack.top().hash;
                m_stack.pop();
                res.type = 1;
                res.hash = set_union(hash2, hash1);
                m_stack.push(res);
            }
            else if (it->class_register == INTERSEC)
            {
                if (m_stack.size() < 2)
                {
                    
                    cout << "RUNTIME ERROR. line: " << it->str << "\nthere are not enough arguments to apply the INTERSEC" << endl;
                    return;
                }
                Var res;
                hash_set hash1, hash2;
                if (m_stack.top().type != 1)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nTHE OPERATION applies only to HASH" << endl;
                    return;
                }
                hash1 = m_stack.top().hash;
                m_stack.pop();
                if (m_stack.top().type != 1)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nTHE OPERATION applies only to HASH" << endl;
                    return;
                }
                hash2 = m_stack.top().hash;
                m_stack.pop();
                res.type = 1;
                res.hash = set_intersec(hash2, hash1);
                m_stack.push(res);
            }
            else if (it->class_register == ITEM)
            {
                if (m_stack.size() < 1)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nthere are not enough arguments to apply the ITEM" << endl;
                    return;
                }
                if (m_stack.top().type != 1)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nTHE OPERATION applies only to HASH" << endl;
                    return;
                }
                if (it->const_flag == 0)
                {
                    Var k;
                    k.type = 0;
                    auto iter = FindElem(t, it->name);
                    if (iter == t.end())
                    {
                        cout << "RUNTIME ERROR. line: " << it->str << "\nthere is not such an argument" << endl;
                        return;
                    }
                    if (iter->type != 0)
                    {
                        cout << "RUNTIME ERROR. line: " << it->str << "\nTHE OPERATION does not apply to HASH" << endl;
                        return;
                    }
                    k.num = m_stack.top().hash.contain(std::any_cast<int>(iter->val));
                    m_stack.pop();
                    m_stack.push(k);
                }
                else if (it->const_flag == 1)
                {
                    Var k;
                    k.type = 0;
                    k.num = m_stack.top().hash.contain(it->value);
                    m_stack.pop();
                    m_stack.push(k);
                }
            }
            else if (it->class_register == POWER)
            {
                if (m_stack.size() < 1)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nthere are not enough arguments to apply the POWER" << endl;
                    return;
                }
                if (m_stack.top().type != 1)
                {
                    cout << "RUNTIME ERROR. line: " << it->str << "\nTHE OPERATION applies only to HASH" << endl;
                    return;
                }
                int res = m_stack.top().hash.power();
                m_stack.pop();
                Var k;
                k.type = 0;
                k.num = res;
                m_stack.push(k);
            }
        }
    }
    ~processing() {}
};

#endif