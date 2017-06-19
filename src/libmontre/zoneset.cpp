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
#include <fstream>
#include <algorithm>
#include <string.h>
#include <cstdint>


#include "zone2.h"
#include "zoneset.h"
#include "interval.h"
#include "boolean.h"

struct earlier_bmin {
    inline bool operator() (const zone& z1, const zone& z2){
        return (z1[1] > z2[1]);
    }
};
struct earlier_emin {
    inline bool operator() (const zone& z1, const zone& z2){
        return (z1[2] > z2[2]);
    }
};

std::ostream& operator<<(std::ostream& os, const zone_set& zs)
{
    for (auto z : zs){
        os << z << std::endl;
    }
    return os;
}

//int64_t charset_encode(const char* word){
//  unsigned int64_t set = 0;
//  for (size_t i = 0; i < strlen(word); i++){
//    set |= 1 << (word[i]-'a');
//  }
//  return set;
//}

zone_set* zs_create(void){
    return new zone_set;
}

void zs_destroy(zone_set *zs){
    delete zs;
}

void zs_filter(zone_set &_retvalue, zone_set &zs){

    std::sort(zs.begin(), zs.end(), earlier_bmin());

    std::vector<zone> active, active_temp;

    for(auto z1 : zs){

        bool already_included = std::any_of(active.begin(), active.end(), [&z1](zone &z2){return zn_included(z1, z2);});

        if(!already_included){

            active.erase( std::remove_if(active.begin(), active.end(), [&z1](zone &z2){return zn_included(z2, z1);}), active.end());
            active.push_back(z1);

            active_temp.clear();
            for(auto z2 : active){
                if( -z2[1] < -z1[1]){
                    _retvalue.push_back(z2);
                }
                else {
                    active_temp.push_back(z2);
                }
            }
            active = active_temp;
        }
    }
    for(auto z2 : active){
        _retvalue.push_back(z2);
    }
}

void zs_intersect(zone_set &_retvalue, zone_set &zs1, zone_set &zs2){

    std::vector<zone> act_1, act_2, act_r, act_r_temp;

    zone kid; // A temporary storage for new-born zones

    std::sort(zs1.begin(), zs1.end(), earlier_bmin());
    std::sort(zs2.begin(), zs2.end(), earlier_bmin());

    size_t i = 0, j = 0;
    while (i < zs1.size() && j < zs2.size()) {

        //std::cout << zs1[i] << " v " << zs2[j] << std::endl;
        if (zs1[i][2] > zs2[j][1]){ //  z1.emin < z2.bmin
            act_1.push_back(zs1[i]);
            act_2.erase(std::remove_if(act_2.begin(), act_2.end(), [&](zone z2){return z2[3] < -zs1[i][1];}), act_2.end()); // remove if z2.bmax < z1.emin
            for(auto z2 : act_2){
                if(zn_intersect(kid, zs1[i], z2)){
                    if(!std::any_of(act_r.begin(), act_r.end(), [&kid](zone &zr){return zn_included(kid, zr);})){
                        act_r.erase( std::remove_if(act_r.begin(), act_r.end(), [&kid](zone &zr){return zn_included(zr, kid);}), act_r.end());
                        act_r.push_back(kid);

                        act_r_temp.clear();
                        for(auto zr : act_r){
                            if( zr[3] < -zs1[i][1]){
                                _retvalue.push_back(zr);
                            }
                            else {
                                act_r_temp.push_back(zr);
                            }
                        }
                        act_r = act_r_temp;
                    }
                };
            }
            ++i; // Dont forget to proceed
        }
        else {
            act_2.push_back(zs2[j]);
            act_1.erase(std::remove_if(act_1.begin(), act_1.end(), [&](zone z1){return z1[3] < -zs2[j][1];}), act_1.end()); // remove if z1.emax < z2.bmin
            for(auto z1 : act_1){
                if(zn_intersect(kid, z1, zs2[j])){
                    if(!std::any_of(act_r.begin(), act_r.end(), [&kid](zone &zr){return zn_included(kid, zr);})){
                        act_r.erase( std::remove_if(act_r.begin(), act_r.end(), [&kid](zone &zr){return zn_included(zr, kid);}), act_r.end());
                        act_r.push_back(kid);

                        act_r_temp.clear();
                        for(auto zr : act_r){
                            if( zr[3] < -zs2[j][1]){
                                _retvalue.push_back(zr);
                            }
                            else {
                                act_r_temp.push_back(zr);
                            }
                        }
                        act_r = act_r_temp;
                    }
                };
            }
            ++j; // Dont forget to proceed
        }
    }
    while (i < zs1.size() && act_2.size() > 0) {
        act_2.erase(std::remove_if(act_2.begin(), act_2.end(), [&](zone z2){return z2[3] < -zs1[i][1];}), act_2.end());
            for(auto z2 : act_2){
                if(zn_intersect(kid, zs1[i], z2)){
                    if(!std::any_of(act_r.begin(), act_r.end(), [&kid](zone &zr){return zn_included(kid, zr);})){
                        act_r.erase( std::remove_if(act_r.begin(), act_r.end(), [&kid](zone &zr){return zn_included(zr, kid);}), act_r.end());
                        act_r.push_back(kid);

                        act_r_temp.clear();
                        for(auto zr : act_r){
                            if( zr[3] < -zs1[i][1]){
                                _retvalue.push_back(zr);
                            }
                            else {
                                act_r_temp.push_back(zr);
                            }
                        }
                        act_r = act_r_temp;
                    }
                }
            }
        ++i;
    }

    while (j < zs2.size() && act_1.size() > 0) {
        act_1.erase(std::remove_if(act_1.begin(), act_1.end(), [&](zone z1){return z1[3] < -zs2[j][1];}), act_1.end()); // remove if z1.emax < z2.bmin
            for(auto z1 : act_1){
                if(zn_intersect(kid, z1, zs2[j])){
                    if(!std::any_of(act_r.begin(), act_r.end(), [&kid](zone &zr){return zn_included(kid, zr);})){
                        act_r.erase( std::remove_if(act_r.begin(), act_r.end(), [&kid](zone &zr){return zn_included(zr, kid);}), act_r.end());
                        act_r.push_back(kid);

                        act_r_temp.clear();
                        for(auto zr : act_r){
                            if( zr[3] < -zs2[j][1]){
                                _retvalue.push_back(zr);
                            }
                            else {
                                act_r_temp.push_back(zr);
                            }
                        }
                        act_r = act_r_temp;
                    }
                }
            }
        ++j;
    }
    for(auto zr : act_r){
         _retvalue.push_back(zr);
    }
}

void zs_concatenate(zone_set &_retvalue, zone_set &zs1, zone_set &zs2){

                std::vector<zone> act_1, act_2, act_r, act_r_temp;

                zone kid; // A temporary storage for new-born zones

                std::sort(zs1.begin(), zs1.end(), earlier_emin());
                std::sort(zs2.begin(), zs2.end(), earlier_bmin());

                size_t i = 0, j = 0;
                while (i < zs1.size() && j < zs2.size()) {

                    if (zs1[i][2] > zs2[j][1]){ //  z1.emin < z2.bmin
                        act_1.push_back(zs1[i]);
                        act_2.erase(std::remove_if(act_2.begin(), act_2.end(), [&](zone z2){return z2[3] < -zs1[i][2];}), act_2.end()); // remove if z2.bmax < z1.emin
                    for(auto z2 : act_2){
                        if(zn_concatenate(kid, zs1[i], z2)){
                            if(!std::any_of(act_r.begin(), act_r.end(), [&kid](zone &zr){return zn_included(kid, zr);})){
                                act_r.erase( std::remove_if(act_r.begin(), act_r.end(), [&kid](zone &zr){return zn_included(zr, kid);}), act_r.end());
                            act_r.push_back(kid);

                            act_r_temp.clear();
                            for(auto zr : act_r){
                                if( zr[3] < -zs1[i][1]){
                                    _retvalue.push_back(zr);
                                }
                                else {
                                    act_r_temp.push_back(zr);
                                }
                            }
                            act_r = act_r_temp;
                        }
                    };
                }
                ++i; // Dont forget to proceed
            }
            else {
            act_2.push_back(zs2[j]);
            act_1.erase(std::remove_if(act_1.begin(), act_1.end(), [&](zone z1){return z1[6] < -zs2[j][1];}), act_1.end()); // remove if z1.emax < z2.bmin
            for(auto z1 : act_1){
                if(zn_concatenate(kid, z1, zs2[j])){
                    if(!std::any_of(act_r.begin(), act_r.end(), [&kid](zone &zr){return zn_included(kid, zr);})){
                        act_r.erase( std::remove_if(act_r.begin(), act_r.end(), [&kid](zone &zr){return zn_included(zr, kid);}), act_r.end());
                    act_r.push_back(kid);

                    act_r_temp.clear();
                    for(auto zr : act_r){
                        if( zr[3] < -zs2[j][1]){
                            _retvalue.push_back(zr);
                        }
                        else {
                            act_r_temp.push_back(zr);
                        }
                    }
                    act_r = act_r_temp;
                }
            };
            }
            ++j; // Dont forget to proceed
            }
//          std::cout << act_r.size() << std::endl;
            }
            while (i < zs1.size() && act_2.size() > 0) {
                act_2.erase(std::remove_if(act_2.begin(), act_2.end(), [&](zone z2){return z2[3] < -zs1[i][2];}), act_2.end());
            for(auto z2 : act_2){
                if(zn_concatenate(kid, zs1[i], z2)){
                    if(!std::any_of(act_r.begin(), act_r.end(), [&kid](zone &zr){return zn_included(kid, zr);})){
                        act_r.erase( std::remove_if(act_r.begin(), act_r.end(), [&kid](zone &zr){return zn_included(zr, kid);}), act_r.end());
                    act_r.push_back(kid);

                    act_r_temp.clear();
                    for(auto zr : act_r){
                        if( zr[3] < -zs1[i][1]){
                            _retvalue.push_back(zr);
                        }
                        else {
                            act_r_temp.push_back(zr);
                        }
                    }
                    act_r = act_r_temp;
                }
            }
            }
            ++i;
            }
            while (j < zs2.size() && act_1.size() > 0) {
                act_1.erase(std::remove_if(act_1.begin(), act_1.end(), [&](zone z1){return z1[6] < -zs2[j][1];}), act_1.end()); // remove if z1.emax < z2.bmin
            for(auto z1 : act_1){
                if(zn_concatenate(kid, z1, zs2[j])){
                    if(!std::any_of(act_r.begin(), act_r.end(), [&kid](zone &zr){return zn_included(kid, zr);})){
                        act_r.erase( std::remove_if(act_r.begin(), act_r.end(), [&kid](zone &zr){return zn_included(zr, kid);}), act_r.end());
                    act_r.push_back(kid);

                    act_r_temp.clear();
                    for(auto zr : act_r){
                        if( zr[3] < -zs2[j][1]){
                            _retvalue.push_back(zr);
                        }
                        else {
                            act_r_temp.push_back(zr);
                        }
                    }
                    act_r = act_r_temp;
                }
            }
            }
            ++j;
            }
            for(auto zr : act_r){
                _retvalue.push_back(zr);
            }
            }

void zs_union(zone_set &_retvalue, zone_set &zs1, zone_set &zs2){
    zone_set zs;
//    zs.reserve( zs1.size() + zs2.size());
    zs.insert( zs.end(), zs1.begin(), zs1.end());
    zs.insert( zs.end(), zs2.begin(), zs2.end());
    zs_filter(_retvalue, zs);
}

bool zs_includes(zone_set &zs1, zone_set &zs2){

    std::sort(zs1.begin(), zs1.end(), earlier_bmin());
    std::sort(zs2.begin(), zs2.end(), earlier_bmin());

    std::vector<zone> act_1;

    size_t i = 0, j = 0;
    while (i < zs1.size() && j < zs2.size()) {

        if (zs1[i][1] >= zs2[j][1]){ //  z1.bmin < z2.bmin
            act_1.push_back(zs1[i]);
            ++i;
        }
        else {
            act_1.erase( std::remove_if(act_1.begin(), act_1.end(), [&](zone z1){return z1[3] < -zs2[j][1];}), act_1.end()); // remove if z1.bmax < z2.bmin
            //std::cout << zs2[j] << " v " << act_1 << std::endl;
            bool z2_incd = std::any_of(act_1.begin(), act_1.end(), [&](zone z1){return zn_included(zs2[j], z1);});
            if(!z2_incd){
                return false;
            }
            ++j;
        }
    }
    while (j < zs2.size() && act_1.size() > 0) {
        act_1.erase( std::remove_if(act_1.begin(), act_1.end(), [&](zone z1){return z1[3] < -zs2[j][1];}), act_1.end()); // remove if z1.bmax < z2.bmin
        //std::cout << zs2[j] << " v " << act_1 << std::endl;
        bool z2_incd = std::any_of(act_1.begin(), act_1.end(), [&](zone z1){return zn_included(zs2[j], z1);});
        if(!z2_incd){
            return false;
        }
        ++j;
    }
    return true;
}

void zs_restrict(zone_set &_retvalue, zone_set &zs, int64_t dmin, int64_t dmax){
    zone kid;
    for(auto z : zs){
        if(zn_restrict(kid, z, dmin, dmax)){
            _retvalue.push_back(kid);
        }
    }
}

void zs_plus(zone_set &_retvalue, zone_set &zs){
    zone_set zplus = zs;
    zone_set zlast = zs;
    zone_set znext, ztemp;
    while(true){
        zs_concatenate(znext, zlast, zs);
        if(!zs_includes(zplus, znext)){
            zlast = znext;
            zs_union(ztemp, zplus, znext);
            zplus = ztemp;
            znext.clear();
            ztemp.clear();
        }
        else
            break;
    }

    _retvalue = zplus;
}

void zs_append(zone_set &_retvalue, int64_t bmin, int64_t bmax, int64_t emin, int64_t emax, int64_t dmin, int64_t dmax){
    zone z = { { 2, bmin, emin, bmax, 1, dmin, emax, dmax, 2 }};
    if(zn_normalize(z)){
        _retvalue.push_back(z);
    }
}
//void zs_append_not_anchored(zone_set &_retvalue, int64_t begin, int64_t end){
//    zone z = { { 2, -begin, -begin, end, 1, 0, end, end-begin, 2 }};
//    _retvalue.push_back(z);
//}
//void zs_append_begin_anchored(zone_set &_retvalue, int64_t begin, int64_t end){
//	zone z = { { 2, -begin, -begin, begin, 1, 0, end, end-begin, 2 } };
//	    _retvalue.push_back(z);
//}
//void zs_append_end_anchored(zone_set &_retvalue, int64_t begin, int64_t end){
//	zone z = { { 2, -begin, -end, end, 1, 0, end, end-begin, 2 } };
//	_retvalue.push_back(z);
//}
//void zs_append_dual_anchored(zone_set &_retvalue, int64_t begin, int64_t end){
//	zone z = { { 2, -begin, -end, begin, 1, 0, end, end-begin, 2 } };
//    _retvalue.push_back(z);
//}

void zs_append_not_anchored(zone_set &_retvalue, int64_t begin, int64_t end){
    zone z = {{ 2,
                zn_make_bound(-begin, ZN_SIGN_LE),
                zn_make_bound(-begin, ZN_SIGN_LE),
                zn_make_bound(end, ZN_SIGN_LE),
                1,
                zn_make_bound(0, ZN_SIGN_LT),
                zn_make_bound(end, ZN_SIGN_LE),
                zn_make_bound(end-begin, ZN_SIGN_LE),
                2 }};
    if(zn_normalize(z)){
        _retvalue.push_back(z);
    }
}
void zs_append_begin_anchored(zone_set &_retvalue, int64_t begin, int64_t end){
    zone z = {{ 2,
                zn_make_bound(-begin, ZN_SIGN_LE),
                zn_make_bound(-begin, ZN_SIGN_LE),
                zn_make_bound(begin, ZN_SIGN_LE),
                1,
                zn_make_bound(0, ZN_SIGN_LT),
                zn_make_bound(end, ZN_SIGN_LE),
                zn_make_bound(end-begin, ZN_SIGN_LE),
                2 }};
    if(zn_normalize(z)){
        _retvalue.push_back(z);
    }
}
void zs_append_end_anchored(zone_set &_retvalue, int64_t begin, int64_t end){
    zone z = {{ 2,
                zn_make_bound(-begin, ZN_SIGN_LE),
                zn_make_bound(-end, ZN_SIGN_LE),
                zn_make_bound(end, ZN_SIGN_LE),
                1,
                zn_make_bound(0, ZN_SIGN_LT),
                zn_make_bound(end, ZN_SIGN_LE),
                zn_make_bound(end-begin, ZN_SIGN_LE),
                2 }};
    if(zn_normalize(z)){
        _retvalue.push_back(z);
    }
}
void zs_append_dual_anchored(zone_set &_retvalue, int64_t begin, int64_t end){
    zone z = {{ 2,
                zn_make_bound(-begin, ZN_SIGN_LE),
                zn_make_bound(-end, ZN_SIGN_LE),
                zn_make_bound(begin, ZN_SIGN_LE),
                1,
                zn_make_bound(end-begin, ZN_SIGN_LT),
                zn_make_bound(end, ZN_SIGN_LE),
                zn_make_bound(end-begin, ZN_SIGN_LE),
                2 }};
    if(zn_normalize(z)){
        _retvalue.push_back(z);
    }
}
void zs_append_point_zone(zone_set &_retvalue, int64_t point){
    zone z = {{ 2,
                zn_make_bound(-point, ZN_SIGN_LE),
                zn_make_bound(-point, ZN_SIGN_LE),
                zn_make_bound(point, ZN_SIGN_LE),
                1,
                zn_make_bound(0, ZN_SIGN_LE),
                zn_make_bound(point, ZN_SIGN_LE),
                zn_make_bound(0, ZN_SIGN_LE),
                2 }};
    if(zn_normalize(z)){
        _retvalue.push_back(z);
    }
}
void zs_append_endmarker_zone(zone_set &_retvalue, int64_t point){
    zone z = {{ 2,
                zn_make_bound(-(point-1), ZN_SIGN_LE),
                zn_make_bound(-point, ZN_SIGN_LE),
                zn_make_bound(point, ZN_SIGN_LE),
                1,
                zn_make_bound(0, ZN_SIGN_LT),
                zn_make_bound(point, ZN_SIGN_LE),
                zn_make_bound(1, ZN_SIGN_LE),
                2 }};
    if(zn_normalize(z)){
        _retvalue.push_back(z);
    }
}
// extern "C" void zs_file_from_file(zone_set &_retvalue, const char* filename, const int64_t sym_wanted){

//     std::string filename_str(filename);
//     std::ifstream infile(filename_str);

//     int64_t now = 0;
//     int64_t next = 0;
//     int64_t duration, sym_read;

//     while (infile >> duration >> sym_read)
//     {
//         next = now + duration;
// //        if((sym_wanted & sym_read) == sym_wanted){
//         if(sym_read == sym_wanted){
//             zs_append(_retvalue, now, next, now, next, 0, duration);
//         }
//         now = next;
//     }

// }

// extern "C" void zs_file_not_anchored(zone_set &_retvalue, const char* filename, const int64_t sym_wanted, const bool polarity){

//     std::string filename_str(filename);
//     std::ifstream infile(filename_str);

//     int64_t now = 0, last = 0, duration;
//     int64_t sym_read;
//     std::string symstr_read;

//     bool current;
//     bool previous = ~polarity;

//     while (infile >> duration >> symstr_read){

//         std::cout << last << ":" << now << std::endl;

//         sym_read = charset_encode(symstr_read.c_str());
//         current = (sym_wanted & sym_read) == sym_wanted; // wanted \in read?

//         if(current != previous){
//             if(polarity == previous){
//                 zs_append(_retvalue, last, now, last, now, 0, now-last);
//             }
//             last = now;
//             previous = current;
//         }

//         now += duration;
//     }
//     if(polarity == previous){
//         zs_append(_retvalue, last, now, last, now, 0, now-last);
//     }
// }

int64_t zs_size(zone_set &zs){
    return zs.size();
}

void zs_print(zone_set &zs){
    for (auto z : zs){
        std::cout <<'('<<z[1]<<' '<< z[3] <<' '<<
                         z[2]<<' '<< z[6] <<' '<<
                         z[5]<<' '<< z[7] <<')'<< std::endl;
    }
}

//int64_t charset_subset(unsigned int64_t p, unsigned int64_t set){
//  return ((p & set) == p);
//}

void zs_from_file2(zone_set &_retvalue, const char* filename, const char_set sym_wanted, const bool polarity, void (*funcp)(zone_set&, int64_t, int64_t)){

    std::string filename_str(filename);
    std::ifstream infile(filename_str);

    int64_t now = 0, last = 0, duration;
    char_set sym_read;
    std::string symstr_read;

    bool current;
    bool previous = ~polarity;

    while (infile >> duration >> symstr_read){

        sym_read = charset_encode(symstr_read.c_str());
        current = (sym_wanted & sym_read) == sym_wanted; // wanted \in read?

        if(current != previous){
            if(polarity == previous){
                (*funcp)(_retvalue, last, now);
            }
            last = now;
            previous = current;
        }
        now += duration;
    }
    if(polarity == previous){
        (*funcp)(_retvalue, last, now);
    }
}

void zs_from_file(zone_set &_retvalue, const char* filename, const bool_expr &expr, void (*funcp)(zone_set&, int64_t, int64_t)){

    std::string filename_str(filename);
    std::ifstream infile(filename_str);

    int64_t now = 0, last = 0, duration;
    char_set sym_read;
    std::string symstr_read;

    bool current;
    bool previous = false;

    while (infile >> duration >> symstr_read){

        sym_read = charset_encode(symstr_read.c_str());
        current = bool_expr_eval(expr, sym_read); // wanted \in read?

        if(current != previous){
            if(previous){
                (*funcp)(_retvalue, last, now);
            }
            last = now;
            previous = current;
        }
        now += duration;
    }
    if(previous){
        (*funcp)(_retvalue, last, now);
    }
}

int64_t zs_total_duration(const char* filename){

    std::string filename_str(filename);
    std::ifstream infile(filename_str);

    int64_t sum = 0, duration;
    std::string symstr_read;

    while (infile >> duration >> symstr_read){
        sum += duration;
    }
    return sum;
}

void zs_from_file_na(zone_set &_retvalue, const char* filename, const bool_expr& expr){
    zs_from_file(_retvalue, filename, expr, zs_append_not_anchored);
}
void zs_from_file_ba(zone_set &_retvalue, const char* filename, const bool_expr& expr){
    zs_from_file(_retvalue, filename, expr, zs_append_begin_anchored);
}
void zs_from_file_ea(zone_set &_retvalue, const char* filename, const bool_expr& expr){
    zs_from_file(_retvalue, filename, expr, zs_append_end_anchored);
}
void zs_from_file_da(zone_set &_retvalue, const char* filename, const bool_expr& expr){
    zs_from_file(_retvalue, filename, expr, zs_append_dual_anchored);
}

void zs_fprintf_begin(FILE *stream, zone_set &zs, int64_t window_begin, int64_t window_end){
    intv_set projection;
    zs_project_to_begin_axis(projection, zs);
    zs_fprintf_interval(stream, projection, window_begin, window_end);
}

void zs_fprintf_end(FILE *stream, zone_set &zs, int64_t window_begin, int64_t window_end){
    intv_set projection;
    zs_project_to_end_axis(projection, zs);
    zs_fprintf_interval(stream, projection, window_begin, window_end);
}

void zs_fprintf_zone(FILE *stream, zone_set &zs){
    for (auto z : zs){
        fprintf(stream, "(%ld %ld %ld %ld %ld %ld) %ld%ld%ld%ld%ld%ld\n",
                -zn_value_bound(z[1]), zn_value_bound(z[3]),
                -zn_value_bound(z[2]), zn_value_bound(z[6]),
                -zn_value_bound(z[5]), zn_value_bound(z[7]),

                 zn_sign_bound(z[1]), zn_sign_bound(z[3]),
                 zn_sign_bound(z[2]), zn_sign_bound(z[6]),
                 zn_sign_bound(z[5]), zn_sign_bound(z[7])
                );

    }
}


int64_t* zs_get_zone(zone_set &zs, int64_t i){

    // int64_t _retvalue[6];

    // _retvalue[0] = -zn_value_bound(zs[i][1]); 
    // _retvalue[1] = zn_value_bound(zs[i][3]);
    // _retvalue[2] = -zn_value_bound(zs[i][2]); 
    // _retvalue[3] = zn_value_bound(zs[i][6]);
    // _retvalue[4] = -zn_value_bound(zs[i][5]);
    // _retvalue[5] = zn_value_bound(zs[i][7]);

    // std::cout <<_retvalue[0]<<' '<< _retvalue[1] <<' '<<
    //             _retvalue[2]<<' '<< _retvalue[3] <<' '<<
    //             _retvalue[4]<<' '<< _retvalue[5] << std::endl;

    return zs[i].data();
}

//void zs_from_file_na_false(zone_set &_retvalue, const char* filename, const char_set sym_wanted){
//    zs_from_file(_retvalue, filename, sym_wanted, false, zs_append_not_anchored);
//}
//void zs_from_file_ba_false(zone_set &_retvalue, const char* filename, const char_set sym_wanted){
//    zs_from_file(_retvalue, filename, sym_wanted, false, zs_append_begin_anchored);
//}
//void zs_from_file_ea_false(zone_set &_retvalue, const char* filename, const char_set sym_wanted){
//    zs_from_file(_retvalue, filename, sym_wanted, false, zs_append_end_anchored);
//}
//void zs_from_file_da_false(zone_set &_retvalue, const char* filename, const char_set sym_wanted){
//    zs_from_file(_retvalue, filename, sym_wanted, false, zs_append_dual_anchored);
//}


