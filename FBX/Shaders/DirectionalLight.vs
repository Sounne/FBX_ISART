#version 330

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoords;
layout(location = 3) in vec3 a_Tangent;

uniform float u_Time;

out FragmentData
{
	vec3 v_Normal;		// en local space
	vec3 v_Tangent;		// en local space
	vec2 v_TexCoords;
	flat vec3 v_I;		// en local space
	vec3 v_V;			// en local space
} OUT;

// en World Space
const vec3 c_LightDirection = vec3(0.0,0.0,-1.0);
const vec3 c_CameraPosition = vec3(0.0,0.0,10.0);

void main(void)
{	
	OUT.v_Normal = a_Normal;
	OUT.v_Tangent = a_Tangent;
	OUT.v_TexCoords = a_TexCoords;


	// ROTATION 3D (pivot Y)
	mat4 rotationMatrix = mat4(
					cos(u_Time), 0.0, -sin(u_Time), 0.0, 	// 1ere colonne					
					0.0, 1.0, 0.0, 0.0,						// 2eme colonne
					sin(u_Time), 0.0, cos(u_Time), 0.0,		// 3eme colonne
					0.0, 0.0, 0.0, 1.0						// 4eme colonne
				);
				
	mat4 translationMatrix = mat4(
						1.0, 0.0, 0.0, 0.0,
						0.0, 1.0, 0.0, 0.0,
						0.0, 0.0, 1.0, 0.0,
						0.0, -2.0, -10.0, 1.0
				);				

	mat4 worldMatrix = translationMatrix;// * rotationMatrix;
	
	// Projection perspective
	float width = 1280.0; float height = 720.0;
	float aspectRatio = width / height;		// pour que les pixels soient "carres"
	float fovY = radians(45.0);				// degree d'ouverture = field of view
	float near = 0.1;						// distance minimale avant laquelle on "clip" le rendu
	float far = 1000.0;						// distance maximale apres laquelle on "clip" le rendu
	
	float d = 1.0 / tan(fovY/2.0);			// distance focale (oeil->plan de projection)
	float range = 1.0 / (near - far);		// normalisation en NDC de la profondeur
	mat4 projectionMatrix = mat4(
						d / aspectRatio, 0.0, 0.0, 0.0,
						0.0, d, 0.0, 0.0,
						0.0, 0.0, (near+far)*range, -1.0,
						0.0, 0.0, (near*far*2.0)*range, 0.0
					);

	vec4 positionWorld = worldMatrix * a_Position;
	
	mat4 inverseWorldMatrix = inverse(worldMatrix);
	// calcul de la direction VERS la camera en local space
	vec4 CamPosLocal = inverseWorldMatrix * vec4(c_CameraPosition, 1.0);
	vec3 V = normalize(vec3(CamPosLocal - a_Position));
	// calcul du vecteur incident I en local space
	vec3 LightDirLocal = mat3(inverseWorldMatrix) * c_LightDirection;
	vec3 I = normalize(LightDirLocal);

	OUT.v_I = I;
	OUT.v_V = V;

	gl_Position = projectionMatrix * positionWorld;
}
