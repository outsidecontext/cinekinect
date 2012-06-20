varying vec2 texCoord;

void main(void) {
	
	texCoord = vec2(gl_Vertex.xy);
	gl_Position = vec4(gl_Vertex.xy * 2.0 - 1.0, 0.0, 1.0);
    
    /*
    gl_TexCoord[0] = gl_MultiTexCoord0;
    texCoord = vec2(gl_MultiTexCoord0.xy);
	gl_Position = ftransform();
     */
	
}