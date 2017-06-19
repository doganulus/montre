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
#include <deque>
#include <iostream>
#include <cstdint>

#include "zone2.h"
#include "boolean.h"

#ifndef TYPEDEF_ZONE_SET_H
#define TYPEDEF_ZONE_SET_H

typedef std::vector<zone> zone_set;

#endif

#ifndef ZONESET_H
#define ZONESET_H

std::ostream& operator<<(std::ostream& os, const zone& z);
std::ostream& operator<<(std::ostream& os, const zone_set& zs);

extern "C" zone_set* zs_create(void);

extern "C" void zs_destroy(zone_set*);

extern "C" void zs_append(zone_set &_retvalue, int64_t bmin, int64_t bmax, int64_t emin, int64_t emax, int64_t dmin, int64_t dmax);

extern "C" void zs_append_not_anchored(zone_set &_retvalue, int64_t begin, int64_t end);
extern "C" void zs_append_begin_anchored(zone_set &_retvalue, int64_t begin, int64_t end);
extern "C" void zs_append_end_anchored(zone_set &_retvalue, int64_t begin, int64_t end);
extern "C" void zs_append_dual_anchored(zone_set &_retvalue, int64_t begin, int64_t end);
extern "C" void zs_append_point_zone(zone_set &_retvalue, int64_t point);
extern "C" void zs_append_endmarker_zone(zone_set &_retvalue, int64_t point);

extern "C" bool zs_includes(zone_set &zs1, zone_set &zs2);

extern "C" void zs_filter(zone_set &_retvalue, zone_set &zs);

extern "C" void zs_restrict(zone_set &_retvalue, zone_set &zs, int64_t dmin, int64_t dmax);

extern "C" void zs_intersect(zone_set &_retvalue, zone_set &zs1, zone_set &zs2);

extern "C" void zs_concatenate(zone_set &_retvalue, zone_set &zs1, zone_set &zs2);

extern "C" void zs_plus(zone_set &_retvalue, zone_set &zs);

extern "C" void zs_union(zone_set &_retvalue, zone_set &zs1, zone_set &zs2);

extern "C" void zs_from_file_na(zone_set&, const char*, const bool_expr&);
extern "C" void zs_from_file_ba(zone_set&, const char*, const bool_expr&);
extern "C" void zs_from_file_ea(zone_set&, const char*, const bool_expr&);
extern "C" void zs_from_file_da(zone_set&, const char*, const bool_expr&);

extern "C" int64_t* zs_get_zone(zone_set &zs, int64_t i);

// extern "C" void zs_from_file(zone_set &_retvalue, const char * filename, const int64_t sym_wanted);
// extern "C" void zs_from_file_b2i(zone_set &_retvalue, const char* filename, const int64_t sym_wanted, const bool polarity);

extern "C" int64_t zs_size(zone_set &zs);
extern "C" int64_t zs_total_duration(const char* filename);

extern "C" void zs_print(zone_set &zs);
extern "C" void zs_fprintf_begin(FILE *stream, zone_set &zs, int64_t window_begin, int64_t window_end);
extern "C" void zs_fprintf_end(FILE *stream, zone_set &zs, int64_t window_begin, int64_t window_end);
extern "C" void zs_fprintf_zone(FILE *stream, zone_set &zs);


void zs_from_file(zone_set&, const char*, const int64_t, const bool, void (*funcp)(zone_set&, int64_t, int64_t));

#endif
