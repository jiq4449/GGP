#ifndef __SHADER_LIGHTING__
#define __SHADER_LIGHTING__

#define MAX_LIGHTS 128

#define LIGHT_TYPE_DIRECTIONAL	0
#define LIGHT_TYPE_POINT		1
#define LIGHT_TYPE_SPOT			2

#define MAX_SPECULAR_EXPONENT 256.0f

static const float F0_NON_METAL = 0.04f;

struct Light
{
    int Type; // Which kind of light? 0, 1 or 2 (see above)
    float3 Direction; // Directional and Spot lights need a direction
    float Range; // Point and Spot lights have a max range for attenuation
    float3 Position; // Point and Spot lights have a position in space
    float Intensity; // All lights need an intensity
    float3 Color; // All lights need a color
    float SpotFalloff; // Spot lights need a value to define their “cone” size
    float3 Padding; // Purposefully padding to hit the 16-byte boundary
};

float3 GetDirectionToLight(Light light)
{
    return normalize(-light.Direction);
}

float3 GetDirectionToLight(Light light, float3 worldPosition)
{
    return normalize(light.Position - worldPosition);
}

float CalculateDiffuseLight(float3 normal, float3 directionToLight)
{
    return saturate(dot(normal, directionToLight));
}

float CalculateAttenuation(Light light, float3 worldPosition)
{
    float distanceFromLight = distance(light.Position, worldPosition);
	
    return pow(saturate(1.0f - (distanceFromLight * distanceFromLight / (light.Range * light.Range))), 2);
}

float CalculatePhongSpecular(Light light, float3 normal, float3 directionToCamera, float roughness)
{
    if (roughness >= 0.95f)
    {
        return 0.0f;
    }

    float specExponent = (1.0f - roughness) * MAX_SPECULAR_EXPONENT;

    float3 view = normalize(directionToCamera);

    float3 reflection = reflect(light.Direction, normal);

    return pow(saturate(dot(reflection, view)), specExponent);
}

float3 CalculateDirectionalLight(Light light, float3 normal, float3 colorTint, float roughness, float3 worldPosition, float3 cameraPosition)
{
    float3 toLight = GetDirectionToLight(light);

    float3 toCamera = normalize(cameraPosition - worldPosition);

    float diffuse = CalculateDiffuseLight(normal, toLight);

    float specular = CalculatePhongSpecular(light, normal, toCamera, roughness);

    specular *= any(diffuse);

    return (colorTint * diffuse + specular) * light.Intensity * light.Color;
}

float3 CalculatePointLight(Light light, float3 normal, float3 colorTint, float roughness, float3 worldPosition, float3 cameraPosition)
{
    float3 toLight = GetDirectionToLight(light, worldPosition);

    float3 toCamera = normalize(cameraPosition - worldPosition);

    float diffuse = CalculateDiffuseLight(normal, toLight);
	
    float attenuation = CalculateAttenuation(light, worldPosition);

    float specular = CalculatePhongSpecular(light, normal, toCamera, roughness);

    specular *= any(diffuse);

    return (diffuse * colorTint + specular) * attenuation * light.Intensity * light.Color;
}

float3 MapNormals(Texture2D NormalMap, SamplerState Sampler, float2 uv, float3 normal, float3 tangent)
{
    float3 unpackedNormal = NormalMap.Sample(Sampler, uv).rgb * 2 - 1;

    float3 N = normal;
    float3 T = tangent;
    T = normalize(T - N * dot(T, N)); // Gram-Schmidt assumes T&N are normalized!
    float3 B = cross(T, N);
    float3x3 TBN = float3x3(T, B, N);

	// Assumes that input.normal is the normal later in the shader
    return mul(unpackedNormal, TBN); // Note multiplication order
}

#endif