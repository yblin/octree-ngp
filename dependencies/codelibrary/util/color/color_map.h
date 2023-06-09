﻿//
// Copyright 2015-2022 Yangbin Lin. All Rights Reserved.
//
// Author: yblin@jmu.edu.cn (Yangbin Lin)
//
// This file is part of the Code Library.
//

#ifndef CODELIBRARY_UTIL_COLOR_COLOR_MAP_H_
#define CODELIBRARY_UTIL_COLOR_COLOR_MAP_H_

#include <cmath>
#include <random>

#include "codelibrary/base/array.h"
#include "codelibrary/base/clamp.h"
#include "codelibrary/util/color/rgb32_color.h"

namespace cl {

/**
 * Color map.
 */
class ColorMap {
public:
    /**
     * Randomly generate n colors.
     */
    explicit ColorMap(int n, int random_seed = 0) {
        CHECK(n > 0);

        std::mt19937 random(random_seed);
        colors_.resize(n);
        for (int i = 0; i < n; ++i) {
            colors_[i] = RGB32Color(random());
        }
    }

    explicit ColorMap(const Array<RGB32Color>& colors)
        : colors_(colors) {
        CHECK(!colors_.empty());
    }

    explicit ColorMap(const std::string& name, const Array<RGB32Color>& colors)
        : name_(name), colors_(colors) {
        CHECK(!colors_.empty());
    }

    /**
     * Map a integer to color.
     */
    const RGB32Color& operator[] (int x) const {
        CHECK(x >= 0);
        return colors_[x % colors_.size()];
    }

    /**
     * Map a float [0, 1] to color.
     */
    const RGB32Color operator[] (double x) const {
        CHECK(x >= 0.0 && x <= 1.0);

        int n = colors_.size();
        double v = x * colors_.size();
        int l = static_cast<int>(std::floor(v));
        int r = static_cast<int>(std::ceil(v));
        l = Clamp(l, 0, n - 1);
        r = Clamp(r, 0, n - 1);

        if (l == r) return colors_[l];

        const RGB32Color& c1 = colors_[l];
        const RGB32Color& c2 = colors_[r];
        return RGB32Color(static_cast<int>((v - l) * c2.red() +
                                           (r - v) * c1.red()),
                          static_cast<int>((v - l) * c2.green() +
                                           (r - v) * c1.green()),
                          static_cast<int>((v - l) * c2.blue() +
                                           (r - v) * c1.blue()));
    }

    /**
     * Return the number of colors in the color_map.
     */
    int size() const {
        return colors_.size();
    }

    /**
     * Reference:
     *   Moreland K. Diverging Color Maps for Scientific Visualization[M].
     *   Advances in Visual Computing. Springer Berlin Heidelberg, 2009:92-103.
     */
    static const ColorMap& Kenneth() {
        static ColorMap color_map("Kenneth", {
            { 58,  76, 192}, { 61,  80, 196}, { 64,  85, 200}, { 67,  89, 203},
            { 70,  93, 207}, { 73,  98, 211}, { 76, 102, 214}, { 79, 106, 218},
            { 83, 111, 221}, { 86, 115, 224}, { 89, 119, 227}, { 92, 123, 230},
            { 96, 127, 232}, { 99, 131, 235}, {102, 135, 237}, {106, 139, 239},
            {109, 143, 241}, {113, 147, 243}, {116, 151, 245}, {119, 155, 247},
            {123, 158, 248}, {126, 162, 250}, {130, 165, 251}, {133, 169, 252},
            {137, 172, 253}, {141, 175, 253}, {144, 178, 254}, {148, 181, 254},
            {151, 184, 254}, {155, 187, 255}, {158, 190, 254}, {162, 193, 254},
            {165, 195, 254}, {169, 198, 253}, {172, 200, 252}, {175, 202, 251},
            {179, 204, 250}, {182, 206, 249}, {185, 208, 248}, {189, 210, 246},
            {192, 211, 245}, {195, 213, 243}, {198, 214, 241}, {201, 215, 239},
            {204, 216, 237}, {207, 217, 234}, {210, 218, 232}, {212, 219, 229},
            {215, 219, 226}, {218, 220, 223}, {220, 220, 220}, {223, 219, 217},
            {226, 218, 213}, {228, 216, 209}, {230, 215, 206}, {233, 213, 202},
            {235, 211, 198}, {237, 209, 194}, {238, 207, 190}, {240, 205, 186},
            {241, 202, 182}, {242, 200, 178}, {244, 197, 174}, {244, 194, 170},
            {245, 192, 166}, {246, 189, 162}, {246, 186, 158}, {247, 182, 154},
            {247, 179, 150}, {247, 176, 146}, {247, 172, 142}, {246, 169, 138},
            {246, 165, 134}, {245, 161, 130}, {245, 157, 126}, {244, 153, 122},
            {243, 149, 118}, {242, 145, 115}, {240, 141, 111}, {239, 136, 107},
            {237, 132, 103}, {236, 128,  99}, {234, 123,  96}, {232, 118,  92},
            {230, 113,  88}, {227, 109,  85}, {225, 104,  81}, {223,  99,  78},
            {220,  93,  74}, {217,  88,  71}, {214,  83,  68}, {211,  77,  64},
            {208,  72,  61}, {205,  66,  58}, {202,  60,  55}, {198,  53,  52},
            {195,  46,  49}, {191,  39,  46}, {187,  30,  43}, {183,  20,  40},
            {180,   4,  38}});
        return color_map;
    }

    /**
     * Matlab 2015 default colormap: Parula.
     */
    static const ColorMap& Parula() {
        static ColorMap color_map("Parula", {
            { 53,  42, 135}, { 54,  48, 147}, { 54,  55, 160}, { 53,  61, 173},
            { 50,  67, 186}, { 44,  74, 199}, { 32,  83, 212}, { 15,  92, 221},
            {  3,  99, 225}, {  2, 104, 225}, {  4, 109, 224}, {  8, 113, 222},
            { 13, 117, 220}, { 16, 121, 218}, { 18, 125, 216}, { 20, 129, 214},
            { 20, 133, 212}, { 19, 137, 211}, { 16, 142, 210}, { 12, 147, 210},
            {  9, 152, 209}, {  7, 156, 207}, {  6, 160, 205}, {  6, 164, 202},
            {  6, 167, 198}, {  7, 169, 194}, { 10, 172, 190}, { 15, 174, 185},
            { 21, 177, 180}, { 29, 179, 175}, { 37, 181, 169}, { 46, 183, 164},
            { 56, 185, 158}, { 66, 187, 152}, { 77, 188, 146}, { 89, 189, 140},
            {101, 190, 134}, {113, 191, 128}, {124, 191, 123}, {135, 191, 119},
            {146, 191, 115}, {156, 191, 111}, {165, 190, 107}, {174, 190, 103},
            {183, 189, 100}, {192, 188,  96}, {200, 188,  93}, {209, 187,  89},
            {217, 186,  86}, {225, 185,  82}, {233, 185,  78}, {241, 185,  74},
            {248, 187,  68}, {253, 190,  61}, {255, 195,  55}, {254, 200,  50},
            {252, 206,  46}, {250, 211,  42}, {247, 216,  38}, {245, 222,  33},
            {245, 228,  29}, {245, 235,  24}, {246, 243,  19}, {249, 251,  14},
            {250, 251,   7}});
        return color_map;
    }

    /**
     * Jet color map, also known as rainbow.
     */
    static const ColorMap& Jet() {
        static ColorMap color_map("Jet", {
            {  0,   0, 143}, {  0,   0, 159}, {  0,   0, 175}, {  0,   0, 191},
            {  0,   0, 207}, {  0,   0, 223}, {  0,   0, 239}, {  0,   0, 255},
            {  0,  16, 255}, {  0,  32, 255}, {  0,  48, 255}, {  0,  64, 255},
            {  0,  80, 255}, {  0,  96, 255}, {  0, 112, 255}, {  0, 128, 255},
            {  0, 143, 255}, {  0, 159, 255}, {  0, 175, 255}, {  0, 191, 255},
            {  0, 207, 255}, {  0, 223, 255}, {  0, 239, 255}, {  0, 255, 255},
            { 16, 255, 239}, { 32, 255, 223}, { 48, 255, 207}, { 64, 255, 191},
            { 80, 255, 175}, { 96, 255, 159}, {112, 255, 143}, {128, 255, 128},
            {143, 255, 112}, {159, 255,  96}, {175, 255,  80}, {191, 255,  64},
            {207, 255,  48}, {223, 255,  32}, {239, 255,  16}, {255, 255,   0},
            {255, 239,   0}, {255, 223,   0}, {255, 207,   0}, {255, 191,   0},
            {255, 175,   0}, {255, 159,   0}, {255, 143,   0}, {255, 128,   0},
            {255, 112,   0}, {255,  96,   0}, {255,  80,   0}, {255,  64,   0},
            {255,  48,   0}, {255,  32,   0}, {255,  16,   0}, {255,   0,   0},
            {239,   0,   0}, {223,   0,   0}, {207,   0,   0}, {191,   0,   0},
            {175,   0,   0}, {159,   0,   0}, {143,   0,   0}, {128,   0,   0}
        });
        return color_map;
    }

    /**
     * HSV color map.
     */
    static const ColorMap& HSV() {
        static ColorMap color_map("HSV", {
            {255,   0,   0}, {255,  24,   0}, {255,  48,   0}, {255,  72,   0},
            {255,  96,   0}, {255, 120,   0}, {255, 143,   0}, {255, 167,   0},
            {255, 191,   0}, {255, 215,   0}, {255, 239,   0}, {247, 255,   0},
            {223, 255,   0}, {199, 255,   0}, {175, 255,   0}, {151, 255,   0},
            {128, 255,   0}, {104, 255,   0}, { 80, 255,   0}, { 56, 255,   0},
            { 32, 255,   0}, {  8, 255,   0}, {  0, 255,  16}, {  0, 255,  40},
            {  0, 255,  64}, {  0, 255,  88}, {  0, 255, 112}, {  0, 255, 135},
            {  0, 255, 159}, {  0, 255, 183}, {  0, 255, 207}, {  0, 255, 231},
            {  0, 255, 255}, {  0, 231, 255}, {  0, 207, 255}, {  0, 183, 255},
            {  0, 159, 255}, {  0, 135, 255}, {  0, 112, 255}, {  0,  88, 255},
            {  0,  64, 255}, {  0,  40, 255}, {  0,  16, 255}, {  8,   0, 255},
            { 32,   0, 255}, { 56,   0, 255}, { 80,   0, 255}, {104,   0, 255},
            {128,   0, 255}, {151,   0, 255}, {175,   0, 255}, {199,   0, 255},
            {223,   0, 255}, {247,   0, 255}, {255,   0, 239}, {255,   0, 215},
            {255,   0, 191}, {255,   0, 167}, {255,   0, 143}, {255,   0, 120},
            {255,   0,  96}, {255,   0,  72}, {255,   0,  48}, {255,   0,  24}
        });
        return color_map;
    }

    /**
     * Matlab hot color map.
     */
    static const ColorMap& Hot() {
        static ColorMap color_map("Hot", {
            { 0,    0,   0}, { 11,   0,   0}, { 21,   0,   0}, { 32,   0,   0},
            { 43,   0,   0}, { 53,   0,   0}, { 64,   0,   0}, { 74,   0,   0},
            { 85,   0,   0}, { 96,   0,   0}, {106,   0,   0}, {117,   0,   0},
            {128,   0,   0}, {138,   0,   0}, {149,   0,   0}, {159,   0,   0},
            {170,   0,   0}, {181,   0,   0}, {191,   0,   0}, {202,   0,   0},
            {213,   0,   0}, {223,   0,   0}, {234,   0,   0}, {244,   0,   0},
            {255,   0,   0}, {255,  11,   0}, {255,  21,   0}, {255,  32,   0},
            {255,  43,   0}, {255,  53,   0}, {255,  64,   0}, {255,  74,   0},
            {255,  85,   0}, {255,  96,   0}, {255, 106,   0}, {255, 117,   0},
            {255, 128,   0}, {255, 138,   0}, {255, 149,   0}, {255, 159,   0},
            {255, 170,   0}, {255, 181,   0}, {255, 191,   0}, {255, 202,   0},
            {255, 213,   0}, {255, 223,   0}, {255, 234,   0}, {255, 244,   0},
            {255, 255,   0}, {255, 255,  16}, {255, 255,  32}, {255, 255,  48},
            {255, 255,  64}, {255, 255,  80}, {255, 255,  96}, {255, 255, 112},
            {255, 255, 128}, {255, 255, 143}, {255, 255, 159}, {255, 255, 175},
            {255, 255, 191}, {255, 255, 207}, {255, 255, 223}, {255, 255, 239},
            {255, 255, 255}});
        return color_map;
    }

    /**
     * Matlab cool color map.
     */
    static const ColorMap& Cool() {
        static ColorMap color_map("Cool", {
            {  0, 255, 255}, {  4, 251, 255}, {  8, 247, 255}, { 12, 243, 255},
            { 16, 239, 255}, { 20, 235, 255}, { 24, 231, 255}, { 28, 227, 255},
            { 32, 223, 255}, { 36, 219, 255}, { 40, 215, 255}, { 45, 210, 255},
            { 49, 206, 255}, { 53, 202, 255}, { 57, 198, 255}, { 61, 194, 255},
            { 65, 190, 255}, { 69, 186, 255}, { 73, 182, 255}, { 77, 178, 255},
            { 81, 174, 255}, { 85, 170, 255}, { 89, 166, 255}, { 93, 162, 255},
            { 97, 158, 255}, {101, 154, 255}, {105, 150, 255}, {109, 146, 255},
            {113, 142, 255}, {117, 138, 255}, {121, 134, 255}, {125, 130, 255},
            {130, 125, 255}, {134, 121, 255}, {138, 117, 255}, {142, 113, 255},
            {146, 109, 255}, {150, 105, 255}, {154, 101, 255}, {158,  97, 255},
            {162,  93, 255}, {166,  89, 255}, {170,  85, 255}, {174,  81, 255},
            {178,  77, 255}, {182,  73, 255}, {186,  69, 255}, {190,  65, 255},
            {194,  61, 255}, {198,  57, 255}, {202,  53, 255}, {206,  49, 255},
            {210,  45, 255}, {215,  40, 255}, {219,  36, 255}, {223,  32, 255},
            {227,  28, 255}, {231,  24, 255}, {235,  20, 255}, {239,  16, 255},
            {243,  12, 255}, {247,   8, 255}, {251,   4, 255}, {255,   0, 255}
        });
        return color_map;
    }

    /**
     * Matlab spring color map.
     */
    static const ColorMap& Spring() {
        static ColorMap color_map("Spring", {
            {255,   0, 255}, {255,   4, 251}, {255,   8, 247}, {255,  12, 243},
            {255,  16, 239}, {255,  20, 235}, {255,  24, 231}, {255,  28, 227},
            {255,  32, 223}, {255,  36, 219}, {255,  40, 215}, {255,  45, 210},
            {255,  49, 206}, {255,  53, 202}, {255,  57, 198}, {255,  61, 194},
            {255,  65, 190}, {255,  69, 186}, {255,  73, 182}, {255,  77, 178},
            {255,  81, 174}, {255,  85, 170}, {255,  89, 166}, {255,  93, 162},
            {255,  97, 158}, {255, 101, 154}, {255, 105, 150}, {255, 109, 146},
            {255, 113, 142}, {255, 117, 138}, {255, 121, 134}, {255, 125, 130},
            {255, 130, 125}, {255, 134, 121}, {255, 138, 117}, {255, 142, 113},
            {255, 146, 109}, {255, 150, 105}, {255, 154, 101}, {255, 158,  97},
            {255, 162,  93}, {255, 166,  89}, {255, 170,  85}, {255, 174,  81},
            {255, 178,  77}, {255, 182,  73}, {255, 186,  69}, {255, 190,  65},
            {255, 194,  61}, {255, 198,  57}, {255, 202,  53}, {255, 206,  49},
            {255, 210,  45}, {255, 215,  40}, {255, 219,  36}, {255, 223,  32},
            {255, 227,  28}, {255, 231,  24}, {255, 235,  20}, {255, 239,  16},
            {255, 243,  12}, {255, 247,   8}, {255, 251,   4}, {255, 255,   0}
        });
        return color_map;
    }

    /**
     * Matlab summer color map.
     */
    static const ColorMap& Summer() {
        static ColorMap color_map("Summer", {
            {  0, 128, 102}, {  4, 130, 102}, {  8, 132, 102}, { 12, 134, 102},
            { 16, 136, 102}, { 20, 138, 102}, { 24, 140, 102}, { 28, 142, 102},
            { 32, 144, 102}, { 36, 146, 102}, { 40, 148, 102}, { 45, 150, 102},
            { 49, 152, 102}, { 53, 154, 102}, { 57, 156, 102}, { 61, 158, 102},
            { 65, 160, 102}, { 69, 162, 102}, { 73, 164, 102}, { 77, 166, 102},
            { 81, 168, 102}, { 85, 170, 102}, { 89, 172, 102}, { 93, 174, 102},
            { 97, 176, 102}, {101, 178, 102}, {105, 180, 102}, {109, 182, 102},
            {113, 184, 102}, {117, 186, 102}, {121, 188, 102}, {125, 190, 102},
            {130, 192, 102}, {134, 194, 102}, {138, 196, 102}, {142, 198, 102},
            {146, 200, 102}, {150, 202, 102}, {154, 204, 102}, {158, 206, 102},
            {162, 208, 102}, {166, 210, 102}, {170, 212, 102}, {174, 215, 102},
            {178, 217, 102}, {182, 219, 102}, {186, 221, 102}, {190, 223, 102},
            {194, 225, 102}, {198, 227, 102}, {202, 229, 102}, {206, 231, 102},
            {210, 233, 102}, {215, 235, 102}, {219, 237, 102}, {223, 239, 102},
            {227, 241, 102}, {231, 243, 102}, {235, 245, 102}, {239, 247, 102},
            {243, 249, 102}, {247, 251, 102}, {251, 253, 102}, {255, 255, 102}
        });
        return color_map;
    }

    /**
     * Matlab autumn color map.
     */
    static const ColorMap& Autumn() {
        static ColorMap color_map("Autumn", {
            {255,   0,   0}, {255,   4,   0}, {255,   8,   0}, {255,  12,   0},
            {255,  16,   0}, {255,  20,   0}, {255,  24,   0}, {255,  28,   0},
            {255,  32,   0}, {255,  36,   0}, {255,  40,   0}, {255,  45,   0},
            {255,  49,   0}, {255,  53,   0}, {255,  57,   0}, {255,  61,   0},
            {255,  65,   0}, {255,  69,   0}, {255,  73,   0}, {255,  77,   0},
            {255,  81,   0}, {255,  85,   0}, {255,  89,   0}, {255,  93,   0},
            {255,  97,   0}, {255, 101,   0}, {255, 105,   0}, {255, 109,   0},
            {255, 113,   0}, {255, 117,   0}, {255, 121,   0}, {255, 125,   0},
            {255, 130,   0}, {255, 134,   0}, {255, 138,   0}, {255, 142,   0},
            {255, 146,   0}, {255, 150,   0}, {255, 154,   0}, {255, 158,   0},
            {255, 162,   0}, {255, 166,   0}, {255, 170,   0}, {255, 174,   0},
            {255, 178,   0}, {255, 182,   0}, {255, 186,   0}, {255, 190,   0},
            {255, 194,   0}, {255, 198,   0}, {255, 202,   0}, {255, 206,   0},
            {255, 210,   0}, {255, 215,   0}, {255, 219,   0}, {255, 223,   0},
            {255, 227,   0}, {255, 231,   0}, {255, 235,   0}, {255, 239,   0},
            {255, 243,   0}, {255, 247,   0}, {255, 251,   0}, {255, 255,   0}
        });
        return color_map;
    }

    /**
     * Matlab winter color map.
     */
    static const ColorMap& Winter() {
        static ColorMap color_map("Winter", {
            {  0,   0, 255}, {  0,   4, 253}, {  0,   8, 251}, {  0,  12, 249},
            {  0,  16, 247}, {  0,  20, 245}, {  0,  24, 243}, {  0,  28, 241},
            {  0,  32, 239}, {  0,  36, 237}, {  0,  40, 235}, {  0,  45, 233},
            {  0,  49, 231}, {  0,  53, 229}, {  0,  57, 227}, {  0,  61, 225},
            {  0,  65, 223}, {  0,  69, 221}, {  0,  73, 219}, {  0,  77, 217},
            {  0,  81, 215}, {  0,  85, 213}, {  0,  89, 210}, {  0,  93, 208},
            {  0,  97, 206}, {  0, 101, 204}, {  0, 105, 202}, {  0, 109, 200},
            {  0, 113, 198}, {  0, 117, 196}, {  0, 121, 194}, {  0, 125, 192},
            {  0, 130, 190}, {  0, 134, 188}, {  0, 138, 186}, {  0, 142, 184},
            {  0, 146, 182}, {  0, 150, 180}, {  0, 154, 178}, {  0, 158, 176},
            {  0, 162, 174}, {  0, 166, 172}, {  0, 170, 170}, {  0, 174, 168},
            {  0, 178, 166}, {  0, 182, 164}, {  0, 186, 162}, {  0, 190, 160},
            {  0, 194, 158}, {  0, 198, 156}, {  0, 202, 154}, {  0, 206, 152},
            {  0, 210, 150}, {  0, 215, 148}, {  0, 219, 146}, {  0, 223, 144},
            {  0, 227, 142}, {  0, 231, 140}, {  0, 235, 138}, {  0, 239, 136},
            {  0, 243, 134}, {  0, 247, 132}, {  0, 251, 130}, {  0, 255, 128}
        });
        return color_map;
    }

    /**
     * Matlab gray color map.
     */
    static const ColorMap& Gray() {
        static ColorMap color_map("Gray", {
            { 0,   0,    0}, {  0,   0,   0}, {  4,   4,   4}, {  8,   8,   8},
            { 12,  12,  12}, { 16,  16,  16}, { 20,  20,  20}, { 24,  24,  24},
            { 28,  28,  28}, { 32,  32,  32}, { 36,  36,  36}, { 40,  40,  40},
            { 45,  45,  45}, { 49,  49,  49}, { 53,  53,  53}, { 57,  57,  57},
            { 61,  61,  61}, { 65,  65,  65}, { 69,  69,  69}, { 73,  73,  73},
            { 77,  77,  77}, { 81,  81,  81}, { 85,  85,  85}, { 89,  89,  89},
            { 93,  93,  93}, { 97,  97,  97}, {101, 101, 101}, {105, 105, 105},
            {109, 109, 109}, {113, 113, 113}, {117, 117, 117}, {121, 121, 121},
            {125, 125, 125}, {130, 130, 130}, {134, 134, 134}, {138, 138, 138},
            {142, 142, 142}, {146, 146, 146}, {150, 150, 150}, {154, 154, 154},
            {158, 158, 158}, {162, 162, 162}, {166, 166, 166}, {170, 170, 170},
            {174, 174, 174}, {178, 178, 178}, {182, 182, 182}, {186, 186, 186},
            {190, 190, 190}, {194, 194, 194}, {198, 198, 198}, {202, 202, 202},
            {206, 206, 206}, {210, 210, 210}, {215, 215, 215}, {219, 219, 219},
            {223, 223, 223}, {227, 227, 227}, {231, 231, 231}, {235, 235, 235},
            {239, 239, 239}, {243, 243, 243}, {247, 247, 247}, {251, 251, 251},
            {255, 255, 255}});
        return color_map;
    }

    /**
     * Color map for plotting lines.
     */
    static const ColorMap& Lines() {
        static ColorMap color_map("Lines", {
            {  0, 114, 189}, {217,  83,  25}, {237, 177,  32}, {126,  47, 142},
            {119, 172,  48}, { 77, 190, 238}, {162,  20,  47}});
        return color_map;
    }

    /**
     * Prism color map.
     */
    static const ColorMap& Prism() {
        static ColorMap color_map("Prism", {
            {255,   0,   0}, {255, 128,   0}, {255, 255,   0}, {  0, 255,   0},
            {  0,   0, 255}, {170,   0, 255}
        });
        return color_map;
    }

    /**
     * Matplotlib color map: 'Set1'.
     */
    static const ColorMap& Set1() {
        static ColorMap color_map("Set1", {
            {228, 26, 28},  {55, 126, 184}, {77, 175, 74}, {152, 78, 163},
            {255, 127, 0},  {255, 255, 51}, {166, 86, 40}, {247, 129, 191},
            {153, 153, 153}
        });
        return color_map;
    }

    /**
     * Matplotlib color map: 'Set2'.
     */
    static const ColorMap& Set2() {
        static ColorMap color_map("Set2", {
            {104, 195, 165}, {252, 141, 98},  {141, 160, 203}, {231, 138, 195},
            {166, 216,  84}, {255, 217, 47},  {229, 196, 148}, {179, 179, 179}
        });
        return color_map;
    }

    /**
     * Matplotlib color map: 'Set3'.
     */
    static const ColorMap& Set3() {
        static ColorMap color_map("Set3", {
            {141, 211, 199}, {255, 255, 179}, {190, 186, 218}, {251, 128, 144},
            {128, 177, 211}, {253, 180,  98}, {179, 222, 105}, {252, 205, 229},
            {217, 217, 217}, {188, 128, 189}, {204, 235, 197}, {255, 237, 111}
        });
        return color_map;
    }

    /**
     * RdYIGn.
     */
    static const ColorMap& RdYIGn() {
        static ColorMap color_map("RdYIGn", {
            {167, 2, 38},    {171, 6, 38},    {175, 9, 38},    {179, 13, 38},
            {183, 17, 38},   {187, 21, 38},   {190, 24, 39},   {194, 28, 39},
            {198, 32, 39},   {200, 34, 39},   {206, 40, 39},   {208, 41, 39},
            {214, 47, 39},   {216, 49, 40},   {219, 56, 43},   {220, 59, 44},
            {224, 66, 47},   {226, 71, 49},   {227, 73, 51},   {230, 80, 54},
            {233, 85, 56},   {235, 90, 58},   {236, 92, 59},   {239, 99, 63},
            {242, 104, 65},  {244, 109, 67},  {244, 112, 68},  {245, 119, 72},
            {246, 124, 74},  {247, 129, 76},  {247, 132, 78},  {248, 140, 81},
            {249, 145, 83},  {250, 150, 86},  {250, 155, 88},  {251, 160, 91},
            {252, 165, 93},  {252, 168, 94},  {253, 175, 98},  {253, 179, 101},
            {253, 183, 104}, {253, 187, 108}, {253, 191, 111}, {253, 195, 114},
            {253, 199, 118}, {254, 200, 119}, {254, 206, 124}, {254, 210, 127},
            {254, 214, 131}, {254, 218, 134}, {254, 222, 137}, {254, 225, 141},
            {254, 228, 145}, {254, 229, 147}, {254, 233, 153}, {254, 235, 157},
            {254, 237, 161}, {255, 240, 166}, {255, 242, 170}, {255, 245, 174},
            {255, 247, 178}, {255, 248, 180}, {255, 252, 186}, {255, 254, 190},
            {253, 254, 188}, {250, 253, 184}, {247, 252, 180}, {244, 250, 176},
            {241, 249, 172}, {238, 248, 168}, {235, 247, 163}, {232, 245, 159},
            {229, 244, 155}, {226, 243, 151}, {223, 242, 147}, {221, 241, 145},
            {217, 239, 139}, {213, 237, 136}, {209, 236, 134}, {205, 234, 131},
            {201, 232, 129}, {197, 230, 126}, {193, 229, 123}, {189, 227, 121},
            {185, 225, 118}, {181, 223, 116}, {177, 222, 113}, {173, 220, 111},
            {169, 218, 108}, {165, 216, 106}, {160, 214, 105}, {157, 213, 105},
            {150, 210, 104}, {145, 208, 104}, {140, 205, 103}, {135, 203, 103},
            {130, 201, 102}, {125, 199, 101}, {120, 197, 101}, {115, 194, 100},
            {110, 192, 100}, {105, 190, 99},  {99, 188, 98},   {93, 185, 97},
            {87, 182, 95},   {81, 179, 94},   {75, 176, 92},   {72, 174, 92},
            {63, 170, 89},   {57, 167, 88},   {51, 164, 86},   {45, 161, 85},
            {39, 159, 83},   {33, 156, 82},   {27, 153, 80},   {24, 149, 79},
            {22, 145, 77},   {20, 142, 75},   {18, 138, 73},   {16, 134, 71},
            {14, 130, 69},   {12, 127, 67},   {10, 123, 65},   {9, 121, 64},
            {6, 115, 61},    {4, 112, 59},    {2, 108, 57},    {0, 104, 55}
        });
        return color_map;
    }

    const Array<RGB32Color>& colors() const {
        return colors_;
    }

    const std::string& name() const {
        return name_;
    }

private:
    std::string name_;
    Array<RGB32Color> colors_;
};

} // namespace cl

#endif // CODELIBRARY_UTIL_COLOR_COLOR_MAP_H_
