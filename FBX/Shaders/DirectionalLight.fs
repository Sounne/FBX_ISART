#version 330

in FragmentData
{
	vec3 v_Normal;		// en local space
    vec3 v_Tangent;		// en local space
	vec2 v_TexCoords;
	flat vec3 v_I;		// en local space
	vec3 v_V;			// en local space
} IN;

uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_SpecularTexture;

out vec4 o_Color;

void main(void)
{
    vec2 texcoords = vec2(IN.v_TexCoords.x, 1.0 - IN.v_TexCoords.y);

    vec3 diffuseColor = vec3(1.0) * texture(u_DiffuseTexture, texcoords).rgb;
    vec3 specularColor = vec3(1.0);
    float specularMask = texture(u_SpecularTexture, texcoords).r;
    
    // defini une matrice de rotation Tangent-Space -> Local Space
    vec3 T = normalize(IN.v_Tangent);
    vec3 Normal = normalize(IN.v_Normal);
    vec3 B = cross(Normal, T);// bitangent perpendiculaire au plan <T,N>

    mat3 TBN = mat3(T, B, Normal);
    
    vec3 bump = texture(u_NormalTexture, texcoords).rgb * 2.0 - 1.0;
    //vec3 N = normalize(bump);  // suffisant si texture en object space
    // convertie la normale de tangent-space (texture) vers object space
    vec3 N = normalize(TBN * bump);
    
    vec3 I = IN.v_I;        // en flat, donc non interpolé
    vec3 V = normalize(IN.v_V);
    vec3 L = -I;
    
    float NdotL = dot(N, L);
    vec3 R = reflect(I, N);

    vec3 diffuse = diffuseColor * max(NdotL, 0.0);
    float smoothness = specularMask * 
        (NdotL > 0.0 ? pow(max(dot(V, R), 0.0), 64.0) : 0.0);
    vec3 specular = specularColor * smoothness;            
    
    o_Color.rgb = (1.0-smoothness)*diffuse + specular;
    o_Color.a = 1.0;
}
