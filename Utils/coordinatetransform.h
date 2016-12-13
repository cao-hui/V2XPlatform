#pragma once

inline double DegreesMinutesToDecimalDegrees(double dm)
{
    //ddmm.mmmm（度分）格式
    dm /= 100; //dd.mmmmmm
    int integerPart = (int)dm;//dd
    double fractionPart = (dm - integerPart) / 0.6; //0.mmmmmm
    return integerPart + fractionPart;
}
