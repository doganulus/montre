/* 
Copyright (c) 2016 by Dogan Ulus <doganulus@gmail.com>.

This file is part of the monitoring tool Montre.

The Montre is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>. 
*/

#include <vector>
#include <cstdint>

#ifndef TYPEDEF_BOOLS_H
#define TYPEDEF_BOOLS_H

typedef uint64_t char_set;
typedef std::vector<int64_t> bool_expr;

std::ostream& operator<<(std::ostream& os, const bool_expr& expr);

#define LNOT (-1)
#define LAND (-2)
#define LOR  (-3)

extern "C" bool_expr* bool_expr_create(void);
extern "C" void bool_expr_destroy(bool_expr *expr);

extern "C" void bool_expr_append(bool_expr& _retvalue, char_set sym);

extern "C" void bool_expr_op_or(bool_expr& _retvalue, bool_expr& oprd1, bool_expr& oprd2);
extern "C" void bool_expr_op_and(bool_expr& _retvalue, bool_expr& oprd1, bool_expr& oprd2);
extern "C" void bool_expr_op_not(bool_expr& _retvalue, bool_expr& oprd1);

extern "C" bool bool_expr_eval(bool_expr expression, char_set sym);

extern "C" int64_t bool_expr_size(bool_expr& expr);

extern "C" char_set charset_encode(const char *word);
extern "C" bool charset_subset(char_set p, char_set set);

extern "C" void bool_expr_print(bool_expr &expr);


#endif
