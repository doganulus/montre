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
#include <vector>
#include <iostream>
#include <cstdint>
#include <limits.h>

#include "zone2.h"

//std::ostream& operator<<(std::ostream& os, const zone& z)
//{
//    os <<'('<<-z[1]<<' '<< z[3] <<' '<<
//         -z[2]<<' '<< z[6] <<' '<<
//         -z[5]<<' '<< z[7] <<')';
//    return os;
//}

std::ostream& operator<<(std::ostream& os, const zone& z)
{
    os <<'('
       << -zn_value_bound(z[1]) <<' '<< zn_value_bound(z[3]) <<' '<<
          -zn_value_bound(z[2]) <<' '<< zn_value_bound(z[6]) <<' '<<
          -zn_value_bound(z[5]) <<' '<< zn_value_bound(z[7])
       <<')' << ' ' <<
          zn_sign_bound(z[1]) << zn_sign_bound(z[3]) <<
          zn_sign_bound(z[2]) << zn_sign_bound(z[6]) <<
          zn_sign_bound(z[5]) << zn_sign_bound(z[7])

       ;
    return os;
}

//bool zn_normalize(zone &zone){
//    for (int64_t k = 0; k <= 2; k++){
//        for (int64_t i = 0; i <= 2; i++){
//            if (i != k && zone[i * 3 + k] < INT_MAX){
//                if (zone[k * 3 + i] < INT_MAX && zone[i * 3 + k] + zone[k * 3 + i] < 0){
//                    return false;
//                }
//                for (int64_t j = 0; j <= 2; j++){
//                    if (i != j && j != k && (zone[k * 3 + j] < INT_MAX) && (zone[i * 3 + k] + zone[k * 3 + j] < zone[i * 3 + j])){
//                        zone[i * 3 + j] = zone[i * 3 + k] + zone[k * 3 + j];
//                    }
//                }
//            }
//        }
//    }
//    return zone[7] != 0; // dmax is not zero
//}

bool zn_normalize(zone &zone){

  for(int64_t k = 0; k <= 2; k++)
    for(int64_t i = 0; i <= 2; i++)
      if (i != k && zone[i * 3 + k] < INT_MAX) {

    if (zone[k * 3 + i] < INT_MAX && zn_add_bound(zone[i * 3 + k], zone[k * 3 + i]) < zn_make_bound(0, ZN_SIGN_LE))
      return false;

    for (int64_t j = 0; j <= 2; j++)
      if (i != j && j != k && zone[k * 3 + j] < INT_MAX &&
          zn_add_bound(zone[i * 3 + k], zone[k * 3 + j]) < zone[i * 3 + j] )
        zone[i * 3 + j] = zn_add_bound(zone[i * 3 + k], zone[k * 3 + j]);

      }
  return true;

}

bool zn_normalize4(zone4 &zone){

  for(int64_t k = 0; k <= 3; k++)
    for(int64_t i = 0; i <= 3; i++)
      if (i != k && zone[i * 4 + k] < INT_MAX) {

    if (zone[k * 4 + i] < INT_MAX && zn_add_bound(zone[i * 4 + k], zone[k * 4 + i]) < zn_make_bound(0, ZN_SIGN_LE))
      return false;

    for (int64_t j = 0; j <= 3; j++)
      if (i != j && j != k && zone[k * 4 + j] < INT_MAX &&
          zn_add_bound(zone[i * 4 + k], zone[k * 4 + j]) < zone[i * 4 + j] )
        zone[i * 4 + j] = zn_add_bound(zone[i * 4 + k], zone[k * 4 + j]);

      }
  return true;

}

//bool zn_normalize4(zone4 &zone){
//    for (int64_t k = 0; k <= 3; k++){
//        for (int64_t i = 0; i <= 3; i++){
//            if (i != k && zone[i * 4 + k] < INT_MAX){
//                if (zone[k * 4 + i] < INT_MAX && zone[i * 4 + k] + zone[k * 4 + i] < 0){
//                    return false;
//                }
//                for (int64_t j = 0; j <= 3; j++){
//                    if (i != j && j != k && (zone[k * 4 + j] < INT_MAX) && (zone[i * 4 + k] + zone[k * 4 + j] < zone[i * 4 + j])){
//                        zone[i * 4 + j] = zone[i * 4 + k] + zone[k * 4 + j];
//                    }
//                }
//            }
//        }
//    }
//    return zone[9] != 0; // dmax is not zero
//}

bool zn_concatenate(zone &_retvalue, const zone &z1, const zone &z2){

    zone4 _tmp4;

    _tmp4[0] = 3;
    _tmp4[1] = z1[1];
    _tmp4[2] = z2[2];
    _tmp4[3] = zn_min_bound(z2[1], z1[2]);
    _tmp4[4] = z1[3];
    _tmp4[5] = 1;
    _tmp4[6] = INT_MAX;
    _tmp4[7] = z1[5];
    _tmp4[8] = z2[6];
    _tmp4[9] = INT_MAX;
    _tmp4[10] = 2;
    _tmp4[11] = z2[7];
    _tmp4[12] = zn_min_bound(z2[3], z1[6]);
    _tmp4[13] = z1[7];
    _tmp4[14] = z2[5];
    _tmp4[15] = 3;

    if(zn_normalize4(_tmp4)){
        zone z = {{2, _tmp4[1], _tmp4[2], _tmp4[4], 1, _tmp4[6], _tmp4[8], _tmp4[9], 2}};
        _retvalue = z;
        return true;
    } else {
        return false;
    }
}

bool zn_restrict(zone &_retvalue, const zone &z, int64_t dmin, int64_t dmax){

    _retvalue[0] = 2;
    _retvalue[1] = z[1];
    _retvalue[2] = z[2];
    _retvalue[3] = z[3];
    _retvalue[4] = 1;
    _retvalue[5] = zn_min_bound(z[5], zn_make_bound(-dmin, dmin > 0 ? ZN_SIGN_LE : ZN_SIGN_LT));
    _retvalue[6] = z[6];
    _retvalue[7] = zn_min_bound(z[7], zn_make_bound(dmax, dmax > 0 ? ZN_SIGN_LE : ZN_SIGN_LT));
    _retvalue[8] = 2;

    return zn_normalize(_retvalue);
}

bool zn_intersect(std::array<int64_t, 9> &_retvalue, const zone &z1, const zone &z2){

    _retvalue[0] = 2;
    _retvalue[1] = zn_min_bound(z2[1], z1[1]);
    _retvalue[2] = zn_min_bound(z2[2], z1[2]);
    _retvalue[3] = zn_min_bound(z2[3], z1[3]);
    _retvalue[4] = 1;
    _retvalue[5] = zn_min_bound(z2[5], z1[5]);
    _retvalue[6] = zn_min_bound(z2[6], z1[6]);
    _retvalue[7] = zn_min_bound(z2[7], z1[7]);
    _retvalue[8] = 2;

    return zn_normalize(_retvalue);
}
