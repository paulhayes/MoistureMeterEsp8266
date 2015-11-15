#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

const uint8_t PROGMEM gamma_cols[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

inline uint32_t colorLerp(uint32_t from, uint32_t to, float v){
  float i = 1.0-max(v,0.0);
  byte fromRed = ( from >> 16 ) & 0xff;
  byte fromGreen = ( from >> 8 ) & 0xff;
  byte fromBlue = from & 0xff;
  byte toRed = ( to >> 16 ) & 0xff;
  byte toGreen = ( to >> 8 ) & 0xff;
  byte toBlue = to & 0xff;
  
  uint32_t red = floor( i * fromRed ) + floor( v * toRed );
  uint32_t green = floor( i * fromGreen ) + floor( v * toGreen );
  uint32_t blue = floor( i * fromBlue ) + floor( v * toBlue );
  /*
  if( red > 0xff ) red = 0xff;
  if( green > 0xff ) green = 0xff;
  if( blue > 0xff ) blue = 0xff;
  */
  return (red<<16) | (green<<8) | blue;
}

/*
uint32_t getColor(float p){
  
  //byte r;
  //byte g;
  //byte b;
 // HSV_to_RGB( 360.0*(1.0-p), 100.0, 50.0, &r, &g, &b );
  //return r<<16 | g<<8 | b;
  

  //return 0xff0000;
  return colorLerp( 0x00ff00, 0xff0000, p );
}
*/
inline float iLerp(float inMin,float inMax,float v){
  return ( v - inMin ) / ( inMax - inMin );
}
/*
inline gammaCorrect(){
  r = pgm_read_byte(&gamma[r]);
  g = pgm_read_byte(&gamma[g]);
  b = pgm_read_byte(&gamma[b]);
}


void HSV_to_RGB(float h, float s, float v, byte *r, byte *g, byte *b)
{
  int i,f,p,q,t;
  
  h = max(0.0, min(360.0, h));
  s = max(0.0, min(100.0, s));
  v = max(0.0, min(100.0, v));
  
  s /= 100;
  v /= 100;
  
  if(s == 0) {
    // Achromatic (grey)
    *r = *g = *b = round(v*255);
    return;
  }

  h /= 60; // sector 0 to 5
  i = floor(h);
  f = h - i; // factorial part of h
  p = v * (1 - s);
  q = v * (1 - s * f);
  t = v * (1 - s * (1 - f));
  switch(i) {
    case 0:
      *r = round(255*v);
      *g = round(255*t);
      *b = round(255*p);
      break;
    case 1:
      *r = round(255*q);
      *g = round(255*v);
      *b = round(255*p);
      break;
    case 2:
      *r = round(255*p);
      *g = round(255*v);
      *b = round(255*t);
      break;
    case 3:
      *r = round(255*p);
      *g = round(255*q);
      *b = round(255*v);
      break;
    case 4:
      *r = round(255*t);
      *g = round(255*p);
      *b = round(255*v);
      break;
    default: // case 5:
      *r = round(255*v);
      *g = round(255*p);
      *b = round(255*q);
    }
}
*/
