//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
// [numthreads(16, 8, 8)] = 1024
#define MSAA 16
#define GROUP_SIZE 8
#include "aa\msaa_resolve.hlsli"