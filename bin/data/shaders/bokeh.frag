
// DoF/Bokeh fragment shader
// adapted from a shader by Martins Upitis
// on this post http://artmartinsh.blogspot.com/2010/02/glsl-lens-blur-filter-with-bokeh.html
// original source http://www.pasteall.org/10779



uniform sampler2DRect tex0;
uniform sampler2DRect tex1;

uniform float blurclamp;  // max blur amount
uniform float bias; //aperture - bigger values for shallower depth of field
uniform float focus;  // determines focus

void main()
{
    float aspectratio = 800.0/600.0;
    vec2 aspectcorrect = vec2(1.0,aspectratio);
    vec4 depth1   = texture2DRect(tex1,gl_TexCoord[0].xy );
    //if (depth1.x == 0.0) discard;
    float factor = ( depth1.x - focus );
    vec2 dofblur = vec2 (clamp( factor * bias, -blurclamp, blurclamp ));
    
    
    vec4 col = vec4(0.0);
    
    col += texture2DRect(tex0, gl_TexCoord[0].xy);
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( 0.0,0.4 )*aspectcorrect) * dofblur);
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( 0.15,0.37 )*aspectcorrect) * dofblur);
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( 0.29,0.29 )*aspectcorrect) * dofblur);
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( -0.37,0.15 )*aspectcorrect) * dofblur);       
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( 0.4,0.0 )*aspectcorrect) * dofblur);   
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( 0.37,-0.15 )*aspectcorrect) * dofblur);       
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( 0.29,-0.29 )*aspectcorrect) * dofblur);       
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( -0.15,-0.37 )*aspectcorrect) * dofblur);
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( 0.0,-0.4 )*aspectcorrect) * dofblur); 
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( -0.15,0.37 )*aspectcorrect) * dofblur);
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( -0.29,0.29 )*aspectcorrect) * dofblur);
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( 0.37,0.15 )*aspectcorrect) * dofblur); 
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( -0.4,0.0 )*aspectcorrect) * dofblur); 
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( -0.37,-0.15 )*aspectcorrect) * dofblur);       
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( -0.29,-0.29 )*aspectcorrect) * dofblur);       
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( 0.15,-0.37 )*aspectcorrect) * dofblur);
    
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( 0.15,0.37 )*aspectcorrect) * dofblur*0.9);
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( -0.37,0.15 )*aspectcorrect) * dofblur*0.9);           
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( 0.37,-0.15 )*aspectcorrect) * dofblur*0.9);           
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( -0.15,-0.37 )*aspectcorrect) * dofblur*0.9);
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( -0.15,0.37 )*aspectcorrect) * dofblur*0.9);
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( 0.37,0.15 )*aspectcorrect) * dofblur*0.9);            
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( -0.37,-0.15 )*aspectcorrect) * dofblur*0.9);   
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( 0.15,-0.37 )*aspectcorrect) * dofblur*0.9);   
    
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( 0.29,0.29 )*aspectcorrect) * dofblur*0.7);
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( 0.4,0.0 )*aspectcorrect) * dofblur*0.7);       
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( 0.29,-0.29 )*aspectcorrect) * dofblur*0.7);   
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( 0.0,-0.4 )*aspectcorrect) * dofblur*0.7);     
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( -0.29,0.29 )*aspectcorrect) * dofblur*0.7);
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( -0.4,0.0 )*aspectcorrect) * dofblur*0.7);     
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( -0.29,-0.29 )*aspectcorrect) * dofblur*0.7);   
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( 0.0,0.4 )*aspectcorrect) * dofblur*0.7);
    
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( 0.29,0.29 )*aspectcorrect) * dofblur*0.4);
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( 0.4,0.0 )*aspectcorrect) * dofblur*0.4);       
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( 0.29,-0.29 )*aspectcorrect) * dofblur*0.4);   
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( 0.0,-0.4 )*aspectcorrect) * dofblur*0.4);     
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( -0.29,0.29 )*aspectcorrect) * dofblur*0.4);
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( -0.4,0.0 )*aspectcorrect) * dofblur*0.4);     
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( -0.29,-0.29 )*aspectcorrect) * dofblur*0.4);   
    col += texture2DRect(tex0, gl_TexCoord[0].xy + (vec2( 0.0,0.4 )*aspectcorrect) * dofblur*0.4);       
    
    gl_FragColor = col/41.0;
    gl_FragColor.a = 1.0;
}