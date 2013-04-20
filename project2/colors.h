#ifndef FILE_COLORS_SEEN
#define FILE_COLORS_SEEN
// Color Structure
typedef struct {
  GLfloat r;     // Red
  GLfloat g;     // Green
  GLfloat b;     // Blue
} rgb;

typedef struct {
    GLfloat h;       // angle in degrees
    GLfloat s;       // percent
    GLfloat v;       // percent
} hsv;

// Colors (From Dr. Wellborn)
static rgb skyBlue = {0.529, .807, 1.0};
static rgb offWhite = {0.87, 0.87, 0.87};
static rgb yellow = {1.0, 1.0, 0.};
static rgb aqua = {127.0/255.0, 1.0, 212.0/255.0};
static rgb seaGreen = {0.329, 1.0, 0.6};
static rgb lavenderBlush = {1.0, 240.0/255.0, 245.0/255.0};
static rgb honeydew = {0.941, 1.0, 0.941};
static rgb coral = {1.0, 114.0/255.0, 86.0/255.0};
static rgb magenta = {1.0, 0.0, 1.0};
static rgb navajoWhite = {1.0, 222.0/255.0, 173.0/255.0};
static rgb blue = {0.0, 0.0, 1.0};
static rgb cadetBlue = {152.0/255.0, 245.0/255.0, 1.0};
static rgb green = {0.0, 1.0, 0.0};
static rgb olive = {192.0/255.0, 1.0, 62.0/255.0};
static rgb khaki = {1.0, 246.0/255.0, 143.0/255.0};
static rgb gold = {1.0, 215.0/255.0, 0.0};
static rgb violet = {238.0/255.0, 130.0/255.0, 238.0/255.0};
static rgb red = {1.0, 0.0, 0.0};     // Sun
static rgb paleGreen = {0.6, 0.9, 0.6};
static rgb deepSkyBlue = {0.0, 0.74, 1.0};
static rgb lightSkyBlue = {100.0/255.0, 226.0/255.0, 1.0};
static rgb antiqueWhite = {1.0, 0.93, 0.83};
static rgb peachPuff = {0.93, 0.81, 0.678};
static rgb azure = {0.94, 0.93, 0.93};
static rgb purple = {0.62, 0.125, 0.94};

//non colors
static rgb black = {0.0, 0.0, 0.0};
static rgb white = {1.0, 1.0, 1.0};

//Neon Colors (From Ryan Justus)
static rgb neonGreen = {111.0/255.0, 1.0, 0.0};
static rgb neonBlue = {77.0/255.0, 77.0/255.0, 1.0};
static rgb neonOrange = {1.0, 65.0/255.0, 5.0/255.0};
static rgb neonPurple = {153.0/255.0, 60.0/255.0, 143.0/255.0};


hsv rgb2hsv(rgb in)
{
    hsv         out;
    double      min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min  < in.b ? min  : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max  > in.b ? max  : in.b;

    out.v = max;                                // v
    delta = max - min;
    if( max > 0.0 ) {
        out.s = (delta / max);                  // s
    } else {
        // r = g = b = 0                        // s = 0, v is undefined
        out.s = 0.0;
        out.h = NAN;                            // its now undefined
        return out;
    }
    if( in.r >= max )                           // > is bogus, just keeps compilor happy
        out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
    else
    if( in.g >= max )
        out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
    else
        out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan

    out.h *= 60.0;                              // degrees

    if( out.h < 0.0 )
        out.h += 360.0;

    return out;
}


rgb hsv2rgb(hsv in)
{
    double      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
        if(isnan(in.h)) {   // in.h == NAN
            out.r = in.v;
            out.g = in.v;
            out.b = in.v;
            return out;
        }
        // error - should never happen
        out.r = 0.0;
        out.g = 0.0;
        out.b = 0.0;
        return out;
    }
    hh = in.h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;     
}
#endif
