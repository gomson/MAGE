//-----------------------------------------------------------------------------
// VS Input Structures
//-----------------------------------------------------------------------------

struct VSInputPosition {
	float3 p      : POSITION0;
};

struct VSInputPositionNormal {
	float3 p      : POSITION0;
	float3 n      : NORMAL0;
};

struct VSInputPositionColor {
	float3 p      : POSITION0;
	float4 color  : COLOR0;
};

struct VSInputPositionTexture {
	float3 p      : POSITION0;
	float2 tex    : TEXCOORD0;
};

struct VSInputPositionNormalColor {
	float3 p      : POSITION0;
	float3 n      : NORMAL0;
	float4 color  : COLOR0;
};

struct VSInputPositionNormalTexture {
	float3 p      : POSITION0;
	float3 n      : NORMAL0;
	float2 tex    : TEXCOORD0;
};

struct VSInputPositionColorTexture {
	float3 p      : POSITION0;
	float4 color  : COLOR0;
	float2 tex    : TEXCOORD0;
};

struct VSInputPositionNormalColorTexture {
	float3 p      : POSITION0;
	float3 n      : NORMAL0;
	float4 color  : COLOR0;
	float2 tex    : TEXCOORD0;
};

struct VSInputPositionTextureTexture {
	float3 p      : POSITION0;
	float2 tex    : TEXCOORD0;
	float2 tex2   : TEXCOORD1;
};

//-----------------------------------------------------------------------------
// PS Input Structures
//-----------------------------------------------------------------------------

struct PSInputPosition {
	float4 p      : SV_POSITION;
};

struct PSInputPositionTexture {
	float4 p      : SV_POSITION;
	float2 tex    : TEXCOORD0;
};

struct PSInputPositionColorTexture {
	float4 p      : SV_POSITION;
	float4 color  : COLOR0;
	float2 tex    : TEXCOORD0;
};

struct PSInputPositionNormalTexture {
	float4 p      : SV_POSITION;
	float3 p_view : POSITION0;
	float3 n_view : NORMAL0;
	float2 tex    : TEXCOORD0;
	float2 tex2   : TEXCOORD1;
};

//-----------------------------------------------------------------------------
// OM Input Structures
//-----------------------------------------------------------------------------

struct OMInputDeferred {
	float4 normal   : SV_Target0;
	float4 diffuse  : SV_Target1;
	float4 specular : SV_Target2;
};