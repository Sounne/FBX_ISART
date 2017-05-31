#version 330

in FragmentData
{
	vec3 v_Normal;		// en local space
	vec2 v_TexCoords;
	flat vec3 v_I;		// en local space
	vec3 v_V;			// en local space
} IN;

uniform sampler2D u_DiffuseTexture;

out vec4 o_Color;

void main(void)
{
    vec3 diffuseColor = vec3(1.0);
    vec3 specularColor = vec3(1.0);

    vec3 N = normalize(IN.v_Normal);
    vec3 I = IN.v_I;        // en flat, donc non interpolé
    vec3 V = normalize(IN.v_V);
    vec3 L = -I;
    
    float NdotL = dot(N, L);
    vec3 R = reflect(I, N);

    vec3 diffuse = diffuseColor * max(NdotL, 0.0);
    vec3 specular = specularColor * 
            (NdotL > 0.0 ? pow(max(dot(V, R), 0.0), 64.0) : 0.0);
    
    o_Color.rgb = diffuse + specular;
    o_Color.a = 1.0;
}
