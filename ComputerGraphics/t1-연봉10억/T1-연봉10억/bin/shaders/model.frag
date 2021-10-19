#ifdef GL_ES
   #ifndef GL_FRAGMENT_PRECISION_HIGH   // highp may not be defined
      #define highp mediump
   #endif
   precision highp float; // default precision needs to be defined
#endif

// input from vertex shader
in vec4 epos;
in vec3 norm;
in vec2 tc;

// the only output variable
out vec4 fragColor;

uniform mat4   view_matrix;
uniform float   shininess;
uniform vec4   light_position, Ia, Id, Is;   // light
uniform vec4   Ka, Kd, Ks;               // material properties
uniform vec4	color;

// texture sampler
uniform sampler2D TEX0;   // rock
uniform sampler2D TEX1;
uniform sampler2D TEX2;
uniform sampler2D TEX3; // spaceship
uniform sampler2D TEX4; // item
uniform sampler2D TEX5; // particle
uniform sampler2D TEX6; // saturn
uniform sampler2D TEX7; // saturnRing
uniform sampler2D TEX8; // saturnRingTrans


uniform int mode;

vec4 phong( vec3 l, vec3 n, vec3 h, vec4 Kd )
{
   vec4 Ira = Ka*Ia;                           // ambient reflection
   vec4 Ird = max(Kd*dot(l,n)*Id,0.0);               // diffuse reflection
   vec4 Irs = max(Ks*pow(dot(h,n),shininess)*Is,0.0);   // specular reflection
   return Ira + Ird + Irs;
}

void main()
{
   // light position in the eye space
   vec4 lpos = view_matrix*light_position;

   vec3 n = normalize(norm);   // norm interpolated via rasterizer should be normalized again here
   vec3 p = epos.xyz;         // 3D position of this fragment
   vec3 l = normalize(lpos.xyz-(lpos.a==0.0?vec3(0):p));   // lpos.a==0 means directional light
   vec3 v = normalize(-p);      // eye-epos = vec3(0)-epos
   vec3 h = normalize(l+v);   // the halfway vector


   if(mode == 0){
      vec4 iKd = texture( TEX0, tc );   // Kd from image
      fragColor = phong(l,n,h,iKd);
   }
   else if(mode == 1){
      fragColor = texture(TEX1, tc);
   }
   else if(mode == 2){
      vec4 color = texture(TEX2, tc);
      if (color.r >= 0.8f && color.g >= 0.8f && color.b >= 0.8f)
         discard;
      fragColor = color;
   }
   else if(mode == 3){
      vec4 iKd = texture( TEX3, tc );   // Kd from image
      fragColor = phong(l,n,h,iKd);
   }
   else if(mode == 4){
      vec4 iKd = texture( TEX4, tc );   // Kd from image
      fragColor = phong(l,n,h,iKd);
   }
   else if(mode == 5){
      fragColor = texture( TEX5, tc ); if(fragColor.a < 0.001) discard;
	  fragColor = vec4(fragColor.rgb,fragColor.r)*color; // enable alpha blending
   }
   else if(mode == 6){
      vec4 iKd = texture( TEX6, tc );   // Kd from image
      fragColor = phong(l,n,h,iKd);
   }
   else if(mode == 7){
      // fragColor = texture( TEX7, tc );

      vec4 src = texture( TEX7, tc );  
	  vec4 alpha_texture = texture( TEX8, tc );
		
	  fragColor = phong( l, n, h, src ); 
      fragColor.a = alpha_texture.r;
   }
}