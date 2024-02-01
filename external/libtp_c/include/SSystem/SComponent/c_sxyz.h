#ifndef C_SXYZ_H
#define C_SXYZ_H

#include "../../dolphin/types.h"

struct SVec {
    s16 x, y, z;
};

class csXyz : public SVec {
public:
    /* 80018BD0 */ ~csXyz() {}
    /* inline   */ csXyz() {}
    /* inline   */ csXyz(const csXyz& other) : SVec(other){};
    /* 802673F4 */ csXyz(s16 x, s16 y, s16 z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    /* 80267404 */ csXyz operator+(csXyz&);
    /* 8026745C */ void operator+=(csXyz&);
    /* 80267490 */ csXyz operator-(csXyz&);
    /* 802674E8 */ csXyz operator*(f32);
};

#endif /* C_SXYZ_H */
