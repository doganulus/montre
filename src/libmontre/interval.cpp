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

#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <array>
#include <cstdint>

#include "zone2.h"
#include "interval.h"

struct earlier_begin {
    inline bool operator() (const intv &i1, const intv &i2){
        return (i1.begin < i2.begin) ? true: false;
    }
};


void zs_project_to_begin_axis(intv_set &_retvalue, zone_set &zs){
    intv_set temp;
	for (auto z : zs){
        temp.push_back(intv(-zn_value_bound(z[1]), zn_value_bound(z[3])));
	}
    zs_merge_intervals(_retvalue, temp);
}

void zs_project_to_end_axis(intv_set &_retvalue, zone_set &zs){
    intv_set temp;
	for (auto z : zs){
        temp.push_back(intv(-zn_value_bound(z[2]), zn_value_bound(z[6])));
	}
    zs_merge_intervals(_retvalue, temp);
}

void zs_fprintf_interval(FILE *stream, intv_set &intervals, int64_t window_begin, int64_t window_end){

    int64_t last;

    if (intervals.size() <= 0){
        fprintf(stream, "%ld 0\n", window_end - window_begin);
        last = window_end;
        return;
    }

    if (intervals[0].begin > window_begin){
        fprintf(stream, "%ld 0\n", intervals[0].begin - window_begin);
    }

    fprintf(stream, "%ld 1\n", intervals[0].end - intervals[0].begin);
    last = intervals[0].end;

    for (size_t i = 1 ; i < intervals.size(); ++i){
        fprintf(stream, "%ld 0\n", intervals[i].begin - last);
        fprintf(stream, "%ld 1\n", intervals[i].end - intervals[i].begin);
        last = intervals[i].end;
    }

    if (intervals[intervals.size()-1].end < window_end){
        fprintf(stream, "%ld 0\n", window_end - intervals[intervals.size()-1].end);
    }
}

void zs_merge_intervals(intv_set &_retvalue, intv_set &intervals){
	
	if (intervals.size() <= 0)
        return;
  
    // sort the intervals based on begin time
    std::sort(intervals.begin(), intervals.end(), earlier_begin());
 
    // push the first interval to stack
    _retvalue.push_back(intervals[0]);
 
    // Start from the next interval and merge if necessary
    for (size_t i = 1 ; i < intervals.size(); ++i){
        // get interval from stack top
        intv last = _retvalue.back();
 
        // if current interval is not overlapping with stack top,
        // push it to the stack
        if (last.end < intervals[i].begin){
            _retvalue.push_back( intervals[i] );
        }
        else if (last.end < intervals[i].end){
            last.end = intervals[i].end;
            _retvalue.pop_back();
            _retvalue.push_back(last);
        }
    }
}



