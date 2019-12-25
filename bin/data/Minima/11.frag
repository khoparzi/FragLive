#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 resolution;

vec4 shape(vec2 _st,float sides,float radius,float smoothing){
  vec2 st=_st*2.-1.;
  // Angle and radius from the current pixel
  float a=atan(st.x,st.y)+3.1416;
  float r=(2.*3.1416)/sides;
  float d=cos(floor(.5+a/r)*r-a)*length(st);
  return vec4(vec3(1.-smoothstep(radius,radius+smoothing,d)),1.);
}

vec2 repeat(vec2 _st,float repeatX,float repeatY,float offsetX,float offsetY){
  vec2 st=_st*vec2(repeatX,repeatY);
  st.x+=step(1.,mod(st.y,2.))*offsetX;
  st.y+=step(1.,mod(st.x,2.))*offsetY;
  return fract(st);
}

vec4 color(vec4 c0,float _r,float _g,float _b,float _a){
  vec4 c=vec4(_r,_g,_b,_a);
  vec4 pos=step(0.,c);
  return vec4(mix((1.-c0)*abs(c),c*c0,pos));
}

vec2 scale(vec2 st,float amount){
  vec2 xy=st-vec2(.5);
  xy*=(1./vec2(amount,amount));
  xy+=vec2(.5);
  return xy;
}

vec2 rotate(vec2 st,float _angle,float speed){
  vec2 xy=st-vec2(.5);
  float angle=_angle+speed*time;
  xy=mat2(cos(angle),-sin(angle),sin(angle),cos(angle))*xy;
  xy+=.5;
  return xy;
}

vec2 polar(vec2 st){
    //    vec2 uv = vec2(_st.x + 0.4, _st.y);
    //    return vec2(atan(uv.x, uv.y), length(_st));
    //    st = st - vec2(0.5);
    st-=.5;
    float r=length(st);
    float a=atan(st.y,st.x);
    return r*vec2(cos(a),sin(r));
}

void main(){
  vec2 pos=(gl_FragCoord.xy/resolution.xy)-vec2(.5,.5);
  vec2 st = gl_FragCoord.xy/resolution.xy;
  st=polar(st);
  st = rotate(st, 0., -0.25);
  st=repeat(st,10.,10.,.0,0.6);
  vec4 rect = shape(st, 3., .5, 0.);
  rect = color(rect, 0.9, 0., 0.9, 1.);
  gl_FragColor=rect;
}