#version 330 core

struct Material
{
	sampler2D diffuse;
	float shininess;
};

struct dirLight
{
	vec3 lightDir;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct pointLight
{
	vec3 lightPos;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float attC;
	float attL;
	float attQ;
};

struct spotLight
{
	vec3 lightPos;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float innerRadius;
	float outerRadius;
	float attC;
	float attL;
	float attQ;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

#define MAX_LIGHTS 10		//if this too big everything fuck :)

uniform Material material;
uniform int NUM_DIR_LIGHTS;
uniform int NUM_POINT_LIGHTS;
uniform int NUM_SPOT_LIGHTS;

uniform dirLight directional_lights[MAX_LIGHTS];
uniform pointLight point_lights[MAX_LIGHTS];
uniform spotLight spot_lights[MAX_LIGHTS];

vec3 calculateDirectionalLight(dirLight light, vec3 normal, vec3 viewDir);
vec3 calculatePointLight(pointLight light, vec3 fragPos, vec3 normal, vec3 viewDir);
vec3 calculateSpotLight(spotLight light, vec3 fragPos, vec3 normal, vec3 viewDir);

void main()
{
	vec3 output = vec3(0.0, 0.0, 0.0);

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(FragPos);

	for (int i = 0; i < NUM_DIR_LIGHTS; i++)
	{
		output += calculateDirectionalLight(directional_lights[i], norm, viewDir);
	}


	for (int i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		output += calculatePointLight(point_lights[i], FragPos, norm, viewDir);
	}

	for (int i = 0; i < NUM_SPOT_LIGHTS; i++)
	{
		output += calculateSpotLight(spot_lights[i], FragPos, norm, viewDir);
	}



    FragColor = vec4(output, 1.0);
} 


vec3 calculateDirectionalLight(dirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.lightDir);

	float diff = max(dot(-lightDir, normal), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;

	vec3 reflectDir = reflect(lightDir, normal);
	float spec = pow(max(dot(reflectDir, -viewDir), 0.0), material.shininess);
	//vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;

	vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

	return vec3(ambient + diffuse);
}

vec3 calculatePointLight(pointLight light, vec3 fragPos, vec3 normal, vec3 viewDir)
{

	vec3 lightDir = normalize(fragPos - light.lightPos);
	float diff = max(dot(normal, -lightDir), 0.0);

	vec3 reflectDir = reflect(lightDir, normal);
	float spec = pow(max(dot(-viewDir, reflectDir), 0.0), material.shininess);

	float d = length(light.lightPos - fragPos);
	float att = 1.0 / (light.attC * d + light.attL * d + light.attQ * (d * d));


	vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;
	//vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;



	ambient  *= att;
    diffuse  *= att;
   // specular *= att;
    return (ambient + diffuse );
}

vec3 calculateSpotLight(spotLight light, vec3 fragPos, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(fragPos - light.lightPos);
	float diff = max(dot(normal, -lightDir), 0.0);

	vec3 reflectDir = reflect(lightDir, normal);
	float spec = pow(max(dot(-viewDir, reflectDir), 0.0), material.shininess);

	float d = length(light.lightPos - fragPos);
	float att = 1.0 / (light.attC * d + light.attL * d + light.attQ * (d * d));

	vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;
	//vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;

	ambient  *= att;
    diffuse  *= att;
    //specular *= att;

	float theta = dot(lightDir, light.direction);
	float intensity = clamp((theta - light.outerRadius) / (light.innerRadius - light.outerRadius), 0.0, 1.0);

	ambient  *= intensity;
    diffuse  *= intensity;
    //specular *= intensity;

	return (ambient + diffuse);
}