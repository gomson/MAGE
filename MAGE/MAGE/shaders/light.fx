struct OmniLight {
	// The position of this omni light in camera-space coordinates.
	float4 m_p;
	// The intensity of this omni light.
	float3 m_I;
	// Padding.
	uint m_padding0;
	//  The distance at which intensity falloff ends of this omni light.
	float m_distance_falloff_end;
	// The distance range where intensity falloff occurs of this omni light.
	// distance_falloff_range = distance_falloff_end - distance_falloff_start
	float m_distance_falloff_range;
	// Padding.
	uint2 m_padding1;
};

struct SpotLight {
	// The position of this spotlight in camera-space coordinates.
	float4 m_p;
	// The intensity of this spotlight .
	float3 m_I;
	// The exponent property of this spotlight.
	float m_exponent_property;
	// The (normalized) direction of this spotlight in camera-space coordinates.
	float3 m_d;
	// Padding.
	uint m_padding;
	// The distance at which intensity falloff ends of this spotlight.
	float m_distance_falloff_end;
	// The distance range where intensity falloff occurs of this spotlight.
	// distance_falloff_range = distance_falloff_end - distance_falloff_start
	float m_distance_falloff_range;
	// The cosine of the umbra angle at which intensity falloff ends of this spotlight.
	float m_cos_umbra;
	// The cosine range where intensity falloff occurs of this spotlight.
	// cos_range = cos_penumbra - cos_umbra
	float m_cos_range;
};

// Calculates the distance fall off at a given distance r.
float DistanceFalloff(float r, float r_end, float r_range) {
	return saturate((r_end - r) / r_range);
}

// Calculates the angular fall off at a given angle theta.
float AngularFalloff(float cos_theta, float cos_umbra, float cos_range, float s_exp) {
	return pow(saturate((cos_theta - cos_umbra) / cos_range), s_exp);
}

// Calculates the maximum contribution of the given omni light for given distance r.
float3 OmniLightMaxContribution(OmniLight light, float r) {
	const float df = DistanceFalloff(r, light.m_distance_falloff_end, light.m_distance_falloff_range);
	return df * light.m_I;
}

// Calculates the maximum contribution of the given spotlight for given distance r and l.
float3 SpotLightMaxContribution(SpotLight light, float r, float3 l) {
	const float cos_theta = dot(light.m_d, -l);
	const float df = DistanceFalloff(r, light.m_distance_falloff_end, light.m_distance_falloff_range);
	const float af = AngularFalloff(cos_theta, light.m_cos_umbra, light.m_cos_range, light.m_exponent_property);
	return df * af * light.m_I;
}