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

#include <cstdint>

#ifndef TYPEDEF_ZONE_H
#define TYPEDEF_ZONE_H
typedef std::array<int64_t, 9> zone;
#endif

#ifndef TYPEDEF_ZONE_SET_H
#define TYPEDEF_ZONE_SET_H
typedef std::vector<zone> zone_set;
#endif

#ifndef TYPEDEF_INTERVAL_H
#define TYPEDEF_INTERVAL_H

typedef struct intv {
    int64_t begin, end;

    intv(){}
    intv(int64_t b, int64_t e){
        begin = b;
        end = e;
    }
} intv;

typedef std::vector<intv> intv_set;

#endif


extern "C" void zs_project_to_begin_axis(intv_set &_retvalue, zone_set &zs);
extern "C" void zs_project_to_end_axis(intv_set &_retvalue, zone_set &zs);

void zs_merge_intervals(intv_set &_retvalue, intv_set &intervals);
void zs_fprintf_interval(FILE *stream, intv_set &intervals, int64_t window_begin, int64_t window_end);
