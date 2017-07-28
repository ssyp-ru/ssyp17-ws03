#pragma once
#include "unit.h"

class Enemy: protected Unit {
    protected:
        bool isGrounded;
        std::vector<Vector2f> patrolStack;
    public:
        void patrol(Vector2f where);
        void reset_patrol();
};