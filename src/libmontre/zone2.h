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

#include <array>
#include <cstdint>

#ifndef TYPEDEF_ZONE_H
#define TYPEDEF_ZONE_H

typedef std::array<int64_t, 9> zone;
typedef std::array<int64_t, 16> zone4;

#endif

#ifndef ZONE_H
#define ZONE_H

#define ZN_SIGN_LE \
  (1)

#define ZN_SIGN_LT \
  (0)

#define zn_make_bound(value,sign) \
  (((value) << 1) | (sign))

#define zn_value_bound(bound) \
  ((bound) >> 1)

#define zn_sign_bound(bound) \
  ((bound) & 1)

#define zn_min_bound(bound1, bound2) \
  ((bound1) < (bound2) ? (bound1) : (bound2))

#define zn_max_bound(bound1, bound2) \
  ((bound1) < (bound2) ? (bound2) : (bound1))

#define zn_add_bound(bound1,bound2) \
  ((((bound1) & ~1) + ((bound2) & ~1)) | ((bound1) & (bound2) & 1))

bool zn_normalize(zone&);

bool zn_restrict(zone& _retvalue, const zone&, int64_t dmin, int64_t dmax);

bool zn_intersect(zone& _retvalue, const zone&, const zone&);

bool zn_concatenate(zone& _retvalue, const zone&, const zone&);

inline bool zn_included(const std::array<int64_t, 9> &z1, const std::array<int64_t, 9> &z2){
    return z1[1] <= z2[1] && z1[2] <= z2[2] && z1[3] <= z2[3] && z1[5] <= z2[5] && z1[6] <= z2[6] && z1[7] <= z2[7];
}




#endif // ZONE_H
