#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "material\spectrum.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// System Includes
//-----------------------------------------------------------------------------
#pragma region

namespace mage::color {

    XMGLOBALCONST SRGB AliceBlue            = { 0.941176534f, 0.972549081f, 1.000000000f };
    XMGLOBALCONST SRGB AntiqueWhite         = { 0.980392218f, 0.921568692f, 0.843137324f };
    XMGLOBALCONST SRGB Aqua                 = { 0.000000000f, 1.000000000f, 1.000000000f };
    XMGLOBALCONST SRGB Aquamarine           = { 0.498039246f, 1.000000000f, 0.831372619f };
    XMGLOBALCONST SRGB Azure                = { 0.941176534f, 1.000000000f, 1.000000000f };
    XMGLOBALCONST SRGB Beige                = { 0.960784376f, 0.960784376f, 0.862745166f };
    XMGLOBALCONST SRGB Bisque               = { 1.000000000f, 0.894117713f, 0.768627524f };
    XMGLOBALCONST SRGB Black                = { 0.000000000f, 0.000000000f, 0.000000000f };
    XMGLOBALCONST SRGB BlanchedAlmond       = { 1.000000000f, 0.921568692f, 0.803921640f };
    XMGLOBALCONST SRGB Blue                 = { 0.000000000f, 0.000000000f, 1.000000000f };
    XMGLOBALCONST SRGB BlueViolet           = { 0.541176498f, 0.168627456f, 0.886274576f };
    XMGLOBALCONST SRGB Brown                = { 0.647058845f, 0.164705887f, 0.164705887f };
    XMGLOBALCONST SRGB BurlyWood            = { 0.870588303f, 0.721568644f, 0.529411793f };
    XMGLOBALCONST SRGB CadetBlue            = { 0.372549027f, 0.619607866f, 0.627451003f };
    XMGLOBALCONST SRGB Chartreuse           = { 0.498039246f, 1.000000000f, 0.000000000f };
    XMGLOBALCONST SRGB Chocolate            = { 0.823529482f, 0.411764741f, 0.117647067f };
    XMGLOBALCONST SRGB Coral                = { 1.000000000f, 0.498039246f, 0.313725501f };
    XMGLOBALCONST SRGB CornflowerBlue       = { 0.392156899f, 0.584313750f, 0.929411829f };
    XMGLOBALCONST SRGB Cornsilk             = { 1.000000000f, 0.972549081f, 0.862745166f };
    XMGLOBALCONST SRGB Crimson              = { 0.862745166f, 0.078431375f, 0.235294133f };
    XMGLOBALCONST SRGB Cyan                 = { 0.000000000f, 1.000000000f, 1.000000000f };
    XMGLOBALCONST SRGB DarkBlue             = { 0.000000000f, 0.000000000f, 0.545098066f };
    XMGLOBALCONST SRGB DarkCyan             = { 0.000000000f, 0.545098066f, 0.545098066f };
    XMGLOBALCONST SRGB DarkGoldenrod        = { 0.721568644f, 0.525490224f, 0.043137256f };
    XMGLOBALCONST SRGB DarkGray             = { 0.662745118f, 0.662745118f, 0.662745118f };
    XMGLOBALCONST SRGB DarkGreen            = { 0.000000000f, 0.392156899f, 0.000000000f };
    XMGLOBALCONST SRGB DarkKhaki            = { 0.741176486f, 0.717647076f, 0.419607878f };
    XMGLOBALCONST SRGB DarkMagenta          = { 0.545098066f, 0.000000000f, 0.545098066f };
    XMGLOBALCONST SRGB DarkOliveGreen       = { 0.333333343f, 0.419607878f, 0.184313729f };
    XMGLOBALCONST SRGB DarkOrange           = { 1.000000000f, 0.549019635f, 0.000000000f };
    XMGLOBALCONST SRGB DarkOrchid           = { 0.600000024f, 0.196078449f, 0.800000072f };
    XMGLOBALCONST SRGB DarkRed              = { 0.545098066f, 0.000000000f, 0.000000000f };
    XMGLOBALCONST SRGB DarkSalmon           = { 0.913725555f, 0.588235319f, 0.478431404f };
    XMGLOBALCONST SRGB DarkSeaGreen         = { 0.560784340f, 0.737254918f, 0.545098066f };
    XMGLOBALCONST SRGB DarkSlateBlue        = { 0.282352954f, 0.239215702f, 0.545098066f };
    XMGLOBALCONST SRGB DarkSlateGray        = { 0.184313729f, 0.309803933f, 0.309803933f };
    XMGLOBALCONST SRGB DarkTurquoise        = { 0.000000000f, 0.807843208f, 0.819607913f };
    XMGLOBALCONST SRGB DarkViolet           = { 0.580392182f, 0.000000000f, 0.827451050f };
    XMGLOBALCONST SRGB DeepPink             = { 1.000000000f, 0.078431375f, 0.576470613f };
    XMGLOBALCONST SRGB DeepSkyBlue          = { 0.000000000f, 0.749019623f, 1.000000000f };
    XMGLOBALCONST SRGB DimGray              = { 0.411764741f, 0.411764741f, 0.411764741f };
    XMGLOBALCONST SRGB DodgerBlue           = { 0.117647067f, 0.564705908f, 1.000000000f };
    XMGLOBALCONST SRGB Firebrick            = { 0.698039234f, 0.133333340f, 0.133333340f };
    XMGLOBALCONST SRGB FloralWhite          = { 1.000000000f, 0.980392218f, 0.941176534f };
    XMGLOBALCONST SRGB ForestGreen          = { 0.133333340f, 0.545098066f, 0.133333340f };
    XMGLOBALCONST SRGB Fuchsia              = { 1.000000000f, 0.000000000f, 1.000000000f };
    XMGLOBALCONST SRGB Gainsboro            = { 0.862745166f, 0.862745166f, 0.862745166f };
    XMGLOBALCONST SRGB GhostWhite           = { 0.972549081f, 0.972549081f, 1.000000000f };
    XMGLOBALCONST SRGB Gold                 = { 1.000000000f, 0.843137324f, 0.000000000f };
    XMGLOBALCONST SRGB Goldenrod            = { 0.854902029f, 0.647058845f, 0.125490203f };
    XMGLOBALCONST SRGB Gray                 = { 0.501960814f, 0.501960814f, 0.501960814f };
    XMGLOBALCONST SRGB Green                = { 0.000000000f, 0.501960814f, 0.000000000f };
    XMGLOBALCONST SRGB GreenYellow          = { 0.678431392f, 1.000000000f, 0.184313729f };
    XMGLOBALCONST SRGB Honeydew             = { 0.941176534f, 1.000000000f, 0.941176534f };
    XMGLOBALCONST SRGB HotPink              = { 1.000000000f, 0.411764741f, 0.705882370f };
    XMGLOBALCONST SRGB IndianRed            = { 0.803921640f, 0.360784322f, 0.360784322f };
    XMGLOBALCONST SRGB Indigo               = { 0.294117659f, 0.000000000f, 0.509803951f };
    XMGLOBALCONST SRGB Ivory                = { 1.000000000f, 1.000000000f, 0.941176534f };
    XMGLOBALCONST SRGB Khaki                = { 0.941176534f, 0.901960850f, 0.549019635f };
    XMGLOBALCONST SRGB Lavender             = { 0.901960850f, 0.901960850f, 0.980392218f };
    XMGLOBALCONST SRGB LavenderBlush        = { 1.000000000f, 0.941176534f, 0.960784376f };
    XMGLOBALCONST SRGB LawnGreen            = { 0.486274540f, 0.988235354f, 0.000000000f };
    XMGLOBALCONST SRGB LemonChiffon         = { 1.000000000f, 0.980392218f, 0.803921640f };
    XMGLOBALCONST SRGB LightBlue            = { 0.678431392f, 0.847058892f, 0.901960850f };
    XMGLOBALCONST SRGB LightCoral           = { 0.941176534f, 0.501960814f, 0.501960814f };
    XMGLOBALCONST SRGB LightCyan            = { 0.878431439f, 1.000000000f, 1.000000000f };
    XMGLOBALCONST SRGB LightGoldenrodYellow = { 0.980392218f, 0.980392218f, 0.823529482f };
    XMGLOBALCONST SRGB LightGreen           = { 0.564705908f, 0.933333397f, 0.564705908f };
    XMGLOBALCONST SRGB LightGray            = { 0.827451050f, 0.827451050f, 0.827451050f };
    XMGLOBALCONST SRGB LightPink            = { 1.000000000f, 0.713725507f, 0.756862819f };
    XMGLOBALCONST SRGB LightSalmon          = { 1.000000000f, 0.627451003f, 0.478431404f };
    XMGLOBALCONST SRGB LightSeaGreen        = { 0.125490203f, 0.698039234f, 0.666666687f };
    XMGLOBALCONST SRGB LightSkyBlue         = { 0.529411793f, 0.807843208f, 0.980392218f };
    XMGLOBALCONST SRGB LightSlateGray       = { 0.466666698f, 0.533333361f, 0.600000024f };
    XMGLOBALCONST SRGB LightSteelBlue       = { 0.690196097f, 0.768627524f, 0.870588303f };
    XMGLOBALCONST SRGB LightYellow          = { 1.000000000f, 1.000000000f, 0.878431439f };
    XMGLOBALCONST SRGB Lime                 = { 0.000000000f, 1.000000000f, 0.000000000f };
    XMGLOBALCONST SRGB LimeGreen            = { 0.196078449f, 0.803921640f, 0.196078449f };
    XMGLOBALCONST SRGB Linen                = { 0.980392218f, 0.941176534f, 0.901960850f };
    XMGLOBALCONST SRGB Magenta              = { 1.000000000f, 0.000000000f, 1.000000000f };
    XMGLOBALCONST SRGB Maroon               = { 0.501960814f, 0.000000000f, 0.000000000f };
    XMGLOBALCONST SRGB MediumAquamarine     = { 0.400000036f, 0.803921640f, 0.666666687f };
    XMGLOBALCONST SRGB MediumBlue           = { 0.000000000f, 0.000000000f, 0.803921640f };
    XMGLOBALCONST SRGB MediumOrchid         = { 0.729411781f, 0.333333343f, 0.827451050f };
    XMGLOBALCONST SRGB MediumPurple         = { 0.576470613f, 0.439215720f, 0.858823597f };
    XMGLOBALCONST SRGB MediumSeaGreen       = { 0.235294133f, 0.701960802f, 0.443137288f };
    XMGLOBALCONST SRGB MediumSlateBlue      = { 0.482352972f, 0.407843173f, 0.933333397f };
    XMGLOBALCONST SRGB MediumSpringGreen    = { 0.000000000f, 0.980392218f, 0.603921592f };
    XMGLOBALCONST SRGB MediumTurquoise      = { 0.282352954f, 0.819607913f, 0.800000072f };
    XMGLOBALCONST SRGB MediumVioletRed      = { 0.780392230f, 0.082352944f, 0.521568656f };
    XMGLOBALCONST SRGB MidnightBlue         = { 0.098039225f, 0.098039225f, 0.439215720f };
    XMGLOBALCONST SRGB MintCream            = { 0.960784376f, 1.000000000f, 0.980392218f };
    XMGLOBALCONST SRGB MistyRose            = { 1.000000000f, 0.894117713f, 0.882353008f };
    XMGLOBALCONST SRGB Moccasin             = { 1.000000000f, 0.894117713f, 0.709803939f };
    XMGLOBALCONST SRGB NavajoWhite          = { 1.000000000f, 0.870588303f, 0.678431392f };
    XMGLOBALCONST SRGB Navy                 = { 0.000000000f, 0.000000000f, 0.501960814f };
    XMGLOBALCONST SRGB OldLace              = { 0.992156923f, 0.960784376f, 0.901960850f };
    XMGLOBALCONST SRGB Olive                = { 0.501960814f, 0.501960814f, 0.000000000f };
    XMGLOBALCONST SRGB OliveDrab            = { 0.419607878f, 0.556862772f, 0.137254909f };
    XMGLOBALCONST SRGB Orange               = { 1.000000000f, 0.647058845f, 0.000000000f };
    XMGLOBALCONST SRGB OrangeRed            = { 1.000000000f, 0.270588249f, 0.000000000f };
    XMGLOBALCONST SRGB Orchid               = { 0.854902029f, 0.439215720f, 0.839215755f };
    XMGLOBALCONST SRGB PaleGoldenrod        = { 0.933333397f, 0.909803987f, 0.666666687f };
    XMGLOBALCONST SRGB PaleGreen            = { 0.596078455f, 0.984313786f, 0.596078455f };
    XMGLOBALCONST SRGB PaleTurquoise        = { 0.686274529f, 0.933333397f, 0.933333397f };
    XMGLOBALCONST SRGB PaleVioletRed        = { 0.858823597f, 0.439215720f, 0.576470613f };
    XMGLOBALCONST SRGB PapayaWhip           = { 1.000000000f, 0.937254965f, 0.835294187f };
    XMGLOBALCONST SRGB PeachPuff            = { 1.000000000f, 0.854902029f, 0.725490212f };
    XMGLOBALCONST SRGB Peru                 = { 0.803921640f, 0.521568656f, 0.247058839f };
    XMGLOBALCONST SRGB Pink                 = { 1.000000000f, 0.752941251f, 0.796078503f };
    XMGLOBALCONST SRGB Plum                 = { 0.866666734f, 0.627451003f, 0.866666734f };
    XMGLOBALCONST SRGB PowderBlue           = { 0.690196097f, 0.878431439f, 0.901960850f };
    XMGLOBALCONST SRGB Purple               = { 0.501960814f, 0.000000000f, 0.501960814f };
    XMGLOBALCONST SRGB Red                  = { 1.000000000f, 0.000000000f, 0.000000000f };
    XMGLOBALCONST SRGB RosyBrown            = { 0.737254918f, 0.560784340f, 0.560784340f };
    XMGLOBALCONST SRGB RoyalBlue            = { 0.254901975f, 0.411764741f, 0.882353008f };
    XMGLOBALCONST SRGB SaddleBrown          = { 0.545098066f, 0.270588249f, 0.074509807f };
    XMGLOBALCONST SRGB Salmon               = { 0.980392218f, 0.501960814f, 0.447058856f };
    XMGLOBALCONST SRGB SandyBrown           = { 0.956862807f, 0.643137276f, 0.376470625f };
    XMGLOBALCONST SRGB SeaGreen             = { 0.180392161f, 0.545098066f, 0.341176480f };
    XMGLOBALCONST SRGB SeaShell             = { 1.000000000f, 0.960784376f, 0.933333397f };
    XMGLOBALCONST SRGB Sienna               = { 0.627451003f, 0.321568638f, 0.176470593f };
    XMGLOBALCONST SRGB Silver               = { 0.752941251f, 0.752941251f, 0.752941251f };
    XMGLOBALCONST SRGB SkyBlue              = { 0.529411793f, 0.807843208f, 0.921568692f };
    XMGLOBALCONST SRGB SlateBlue            = { 0.415686309f, 0.352941185f, 0.803921640f };
    XMGLOBALCONST SRGB SlateGray            = { 0.439215720f, 0.501960814f, 0.564705908f };
    XMGLOBALCONST SRGB Snow                 = { 1.000000000f, 0.980392218f, 0.980392218f };
    XMGLOBALCONST SRGB SpringGreen          = { 0.000000000f, 1.000000000f, 0.498039246f };
    XMGLOBALCONST SRGB SteelBlue            = { 0.274509817f, 0.509803951f, 0.705882370f };
    XMGLOBALCONST SRGB Tan                  = { 0.823529482f, 0.705882370f, 0.549019635f };
    XMGLOBALCONST SRGB Teal                 = { 0.000000000f, 0.501960814f, 0.501960814f };
    XMGLOBALCONST SRGB Thistle              = { 0.847058892f, 0.749019623f, 0.847058892f };
    XMGLOBALCONST SRGB Tomato               = { 1.000000000f, 0.388235331f, 0.278431386f };
    XMGLOBALCONST SRGB Turquoise            = { 0.250980407f, 0.878431439f, 0.815686345f };
    XMGLOBALCONST SRGB Violet               = { 0.933333397f, 0.509803951f, 0.933333397f };
    XMGLOBALCONST SRGB Wheat                = { 0.960784376f, 0.870588303f, 0.701960802f };
    XMGLOBALCONST SRGB White                = { 1.000000000f, 1.000000000f, 1.000000000f };
    XMGLOBALCONST SRGB WhiteSmoke           = { 0.960784376f, 0.960784376f, 0.960784376f };
    XMGLOBALCONST SRGB Yellow               = { 1.000000000f, 1.000000000f, 0.000000000f };
    XMGLOBALCONST SRGB YellowGreen          = { 0.603921592f, 0.803921640f, 0.196078449f };
}

#pragma endregion