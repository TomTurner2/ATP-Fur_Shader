# Project Metallurgy - PBR Fur Shader
A fur shader developed for my Advanced Technologies Module, made in Directx11. The shader uses the shelling technique implemented using a geometry shader and uses a Cooke-Torrance BRDF using the same terms used in the Order 1886 for their default materials.

## Features:
- Material framework
- Fur shader using the shelling technique (geometry shader)
- Cooke-Torrance microfacet BRDF
- Obj loading using Assimp
- Texture loading using WIC Texture loader

## Libraries Used:
- Assimp
- WIC Texture Loader
- AntTweakBar

## PBR Terms:
- Normal Distribution Function (NDF) Term - Trowbridge-Reitz
- Geometry Term - GSmith
- Fresnel Term - Schlicks Approximation
