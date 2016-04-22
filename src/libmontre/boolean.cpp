/* 
Copyright (c) 2016 by Dogan Ulus <doganulus@gmail.com>.

This file is part of the monitoring tool Montre.

The Montre is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>. 
*/

#include <stack>
#include <iostream>
#include <string.h>
#include <cstdint>

#include "boolean.h"

bool_expr* bool_expr_create(void){
    return new bool_expr;
}

void bool_expr_destroy(bool_expr *expr){
    delete expr;
}

std::ostream& operator<<(std::ostream& os, const bool_expr& expr)
{
    for (auto sym : expr){
        os << sym << " ";
    }
    return os;
}

void bool_expr_print(bool_expr &expr){
    std::cout << expr << std::endl;
}

void bool_expr_append(bool_expr& _retvalue, char_set sym){
    _retvalue.push_back(sym);
}

void bool_expr_op_or(bool_expr& _retvalue, bool_expr& oprd1, bool_expr& oprd2){
    _retvalue.reserve(oprd1.size() + oprd2.size() + 1);
    _retvalue.insert(_retvalue.end(), oprd1.begin(), oprd1.end());
    _retvalue.insert(_retvalue.end(), oprd2.begin(), oprd2.end());
    _retvalue.push_back(LOR);
}

void bool_expr_op_and(bool_expr& _retvalue, bool_expr& oprd1, bool_expr& oprd2){
    _retvalue.reserve(oprd1.size() + oprd2.size() + 1);
    _retvalue.insert(_retvalue.end(), oprd1.begin(), oprd1.end());
    _retvalue.insert(_retvalue.end(), oprd2.begin(), oprd2.end());
    _retvalue.push_back(LAND);
}

void bool_expr_op_not(bool_expr& _retvalue, bool_expr& oprd1){
    _retvalue.reserve(oprd1.size() + 1);
    _retvalue.insert(_retvalue.end(), oprd1.begin(), oprd1.end());
    _retvalue.push_back(LNOT);
}

char_set charset_encode(const char *word){
  uint64_t set = 0;
  for (size_t i = 0; i < strlen(word); i++){
    set |= 1 << (word[i]-'a');
  }
  return set;
}

bool charset_subset(char_set p, char_set set){
  return p && ((p & set) == p);
}

bool bool_expr_eval(bool_expr expression, char_set sym){

    std::stack<bool> result;
    bool oprd1, oprd2;

    for (std::size_t i = 0 ; i < expression.size(); ++i){
        if(expression[i] > 0){
            result.push(charset_subset(expression[i], sym));
        } else if (expression[i] == LNOT){
            oprd1 = result.top();
            result.pop();
            result.push(not oprd1);
        } else if (expression[i] == LAND){
            oprd1 = result.top();
            result.pop();
            oprd2 = result.top();
            result.pop();
            result.push(oprd1 && oprd2);
        } else if (expression[i] == LOR){
            oprd1 = result.top();
            result.pop();
            oprd2 = result.top();
            result.pop();
            result.push(oprd1 || oprd2);
        }
//        std::cout << "Top:"<< result.top() << std::endl;
    }
    return result.top();
}


int64_t bool_expr_size(bool_expr& expr){
    return expr.size();
}
