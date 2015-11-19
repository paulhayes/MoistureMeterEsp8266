#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define clamp(minV,maxV,v) (minV>v)?minV:((maxV<v)?maxV:v)

inline float iLerp(float inMin,float inMax,float v){
  return ( v - inMin ) / ( inMax - inMin );
}
