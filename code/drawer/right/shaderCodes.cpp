#include "right.h"

namespace DrawerLib
{

String RightDrawer::getMeshVertexShaderCode(MeshVertexShaderIndex params)
{
	const char* str =
	"uniform highp vec4 FullTransformMatrix[4];\n"
	"uniform highp vec4 TexCoordMatrix[3];\n"
	"uniform mediump vec4 ToEyeVectorMatrix[4];\n"
	"uniform mediump vec4 TangentBinormalMatrix[3];\n"
	"uniform mediump vec4 NormalMatrix[3];\n" //20
	"#ifdef LIGHT_PROBE_STAR7\n"
	"	uniform mediump vec4 LightProbeStar7_coordMAdd[2];\n"
	"	uniform mediump vec4 LightProbeStar7_probes[21];\n"
	"#endif\n"
	"#ifdef LIGHT_PROBE_OCTO\n"
	"	uniform mediump vec4 LightProbeOcto_coordMAdd[2];\n"
	"	uniform mediump vec4 LightProbeOcto_probes[24];\n"
	"#endif\n"
	"#ifdef LIGHT_PROBE_SINGLE\n"
	"	uniform mediump vec4 LightProbeSingle_probe[3];\n"
	"#endif\n"
	"#ifdef NO_LIGHTING_INFO\n"
	"	uniform mediump vec3 ToLightDirection;\n"
	"#endif\n"
	"uniform mediump float NoLightBackFacesMultCoef;\n"
	"uniform mediump float NoLightBackFacesAddCoef;\n"
	"#ifdef BONES\n"
	"	uniform mediump vec4 BoneArray[81]; //27 bones \n"
	"#endif\n"
	"#ifdef LIGHT_PROBES\n"
	"	uniform mediump vec4 LightProbeArray[100];// 50 light probes\n"
	"#endif\n"
	"#ifdef HEIGHTMAP\n"
	"	uniform mediump vec4 OpacityCoefs[2];\n"
	"#endif\n"
	""
	"#ifdef HEIGHTMAP\n"
	"	attribute highp vec2 vertPositionXY;\n"
	"	attribute highp float vertHeight;\n"
	"	attribute mediump vec2 vertDerivata;\n"
	"	attribute lowp vec4 vertLayers;\n"
	"#else\n"
	"	attribute highp vec3 vertPosition;\n"
	"	attribute highp vec2 vertTexCoord;\n"
	"	attribute mediump vec4 vertTangentAndUnocclusion;\n"
	"	attribute mediump vec4 vertBinormalAndOpacity;\n"
	"	attribute mediump vec4 vertNormalAndOpacity2;\n"
	"#endif\n"
	"#ifdef BONES\n"
	"	attribute mediump vec4 vertBoneIndexes;\n"
	"	attribute mediump vec4 vertBoneWeights;\n"
	"#endif\n"
	"#ifdef LIGHT_PROBES\n"
	"	attribute mediump vec4 vertLightProbeIndexes;\n"
	"	attribute mediump vec4 vertLightProbeWeights;\n"
	"#endif\n"
	"#ifdef BUILTIN_LIGHTING\n"
	"	attribute mediump vec4 vertToLightVectorXLightIntensivityAndLightIntensivity;\n"
	"	attribute mediump vec4 vertLightColor;\n"
	"	attribute mediump vec4 vertAmbientColor;\n"
	"#endif\n"
	""
	"varying highp vec2 vrTexCoord;\n"
	"varying mediump vec4 vrTangentAndToEyeX;\n"
	"varying mediump vec4 vrBinormalAndToEyeY;\n"
	"varying mediump vec4 vrNormalAndToEyeZ;\n"
	"varying mediump vec4 vrUnocclusionOpacityOpacity2Frontlight;\n"
	"#ifndef NO_LIGHTING_INFO\n"
	"	varying mediump vec4 vrToLightVectorXLightIntensivityAndLightIntensivity;\n"
	"	varying mediump vec4 vrLightColorXEnergyAndEnergy;\n"
	"	varying mediump vec3 vrAmbientColor;\n"
	"#endif\n"
	""
	"void main()\n"
	"{\n"
	"#ifdef HEIGHTMAP\n"
	"	highp vec3 pos = vec3(vertPositionXY, vertHeight);\n"
	"	const mediump float derivata_invscale = 0.00781273842586;\n"
	"	mediump vec3 tangent = vec3(derivata_invscale, 0.0, vertDerivata.x);\n"
	"	mediump vec3 binormal = vec3(0.0, derivata_invscale, vertDerivata.y);\n"
	"	mediump vec3 normal = vec3(-vertDerivata.x, -vertDerivata.y, derivata_invscale);\n"
	"	vrUnocclusionOpacityOpacity2Frontlight.x = 1.0;\n"
	"   vrUnocclusionOpacityOpacity2Frontlight.y = dot(vertLayers, OpacityCoefs[0]);\n"
	"   vrUnocclusionOpacityOpacity2Frontlight.z = dot(vertLayers, OpacityCoefs[1]);\n"
	"	vrTexCoord = TexCoordMatrix[0].xy * vertPositionXY.x + TexCoordMatrix[1].xy * vertPositionXY.y + TexCoordMatrix[2].xy;\n"
	"#else\n"
	"	highp vec3 pos = vertPosition;\n"
	"	mediump vec3 tangent = vertTangentAndUnocclusion.xyz * 2.0 - 1.0;\n"
	"	mediump vec3 binormal = vertBinormalAndOpacity.xyz * 2.0 - 1.0;\n"
	"	mediump vec3 normal = vertNormalAndOpacity2.xyz * 2.0 - 1.0;\n"
	"	vrUnocclusionOpacityOpacity2Frontlight.x = vertTangentAndUnocclusion.w;\n"
	"	vrUnocclusionOpacityOpacity2Frontlight.y = vertBinormalAndOpacity.w;\n"
	"	vrUnocclusionOpacityOpacity2Frontlight.z = vertNormalAndOpacity2.w;\n"
	"	vrTexCoord = TexCoordMatrix[0].xy * vertTexCoord.x + TexCoordMatrix[1].xy * vertTexCoord.y + TexCoordMatrix[2].xy;\n"
	"#endif\n"
	""
	"#ifdef BONES\n"
	"	const mediump float constant_3 = 3.0;\n"
	"	mediump vec4 pos4 = vec4(pos, 1.0);\n"
	"	mediump vec3 tangent3 = tangent;\n"
	"	mediump vec3 binormal3 = binormal;\n"
	"	mediump vec3 normal3 = normal;\n"

	"	mediump vec4 boneMatrixRowX = vertBoneWeights.x * BoneArray[int(vertBoneIndexes.x * constant_3)];\n"
	"	pos.x = dot(boneMatrixRowX, pos4);\n"
	"	tangent.x = dot(boneMatrixRowX.xyz, tangent3);\n"
	"	binormal.x = dot(boneMatrixRowX.xyz, binormal3);\n"
	"	normal.x = dot(boneMatrixRowX.xyz, normal3);\n"
	""
	"	mediump vec4 boneMatrixRowY = vertBoneWeights.x * BoneArray[int(vertBoneIndexes.x * constant_3 + 1.0)];\n"
	"	pos.y = dot(boneMatrixRowY, pos4);\n"
	"	tangent.y = dot(boneMatrixRowY.xyz, tangent3);\n"
	"	binormal.y = dot(boneMatrixRowY.xyz, binormal3);\n"
	"	normal.y = dot(boneMatrixRowY.xyz, normal3);\n"
	""
	"	mediump vec4 boneMatrixRowZ = vertBoneWeights.x * BoneArray[int(vertBoneIndexes.x * constant_3 + 2.0)];\n"
	"	pos.z = dot(boneMatrixRowZ, pos4);\n"
	"	tangent.z = dot(boneMatrixRowZ.xyz, tangent3);\n"
	"	binormal.z = dot(boneMatrixRowZ.xyz, binormal3);\n"
	"	normal.z = dot(boneMatrixRowZ.xyz, normal3);\n"
	""
	"	boneMatrixRowX = vertBoneWeights.y * BoneArray[int(vertBoneIndexes.y * constant_3)];\n"
	"	pos.x += dot(boneMatrixRowX, pos4);\n"
	"	tangent.x += dot(boneMatrixRowX.xyz, tangent3);\n"
	"	binormal.x += dot(boneMatrixRowX.xyz, binormal3);\n"
	"	normal.x += dot(boneMatrixRowX.xyz, normal3);\n"
	""
	"	boneMatrixRowY = vertBoneWeights.y * BoneArray[int(vertBoneIndexes.y * constant_3 + 1.0)];\n"
	"	pos.y += dot(boneMatrixRowY, pos4);\n"
	"	tangent.y += dot(boneMatrixRowY.xyz, tangent3);\n"
	"	binormal.y += dot(boneMatrixRowY.xyz, binormal3);\n"
	"	normal.y += dot(boneMatrixRowY.xyz, normal3);\n"
	""
	"	boneMatrixRowZ = vertBoneWeights.y * BoneArray[int(vertBoneIndexes.y * constant_3 + 2.0)];\n"
	"	pos.z += dot(boneMatrixRowZ, pos4);\n"
	"	tangent.z += dot(boneMatrixRowZ.xyz, tangent3);\n"
	"	binormal.z += dot(boneMatrixRowZ.xyz, binormal3);\n"
	"	normal.z += dot(boneMatrixRowZ.xyz, normal3);\n"
	""
	"	boneMatrixRowX = vertBoneWeights.z * BoneArray[int(vertBoneIndexes.z * constant_3)];\n"
	"	pos.x += dot(boneMatrixRowX, pos4);\n"
	"	tangent.x += dot(boneMatrixRowX.xyz, tangent3);\n"
	"	binormal.x += dot(boneMatrixRowX.xyz, binormal3);\n"
	"	normal.x += dot(boneMatrixRowX.xyz, normal3);\n"
	""
	"	boneMatrixRowY = vertBoneWeights.z * BoneArray[int(vertBoneIndexes.z * constant_3 + 1.0)];\n"
	"	pos.y += dot(boneMatrixRowY, pos4);\n"
	"	tangent.y += dot(boneMatrixRowY.xyz, tangent3);\n"
	"	binormal.y += dot(boneMatrixRowY.xyz, binormal3);\n"
	"	normal.y += dot(boneMatrixRowY.xyz, normal3);\n"
	""
	"	boneMatrixRowZ = vertBoneWeights.z * BoneArray[int(vertBoneIndexes.z * constant_3 + 2.0)];\n"
	"	pos.z += dot(boneMatrixRowZ, pos4);\n"
	"	tangent.z += dot(boneMatrixRowZ.xyz, tangent3);\n"
	"	binormal.z += dot(boneMatrixRowZ.xyz, binormal3);\n"
	"	normal.z += dot(boneMatrixRowZ.xyz, normal3);\n"
	""
	"	boneMatrixRowX = vertBoneWeights.w * BoneArray[int(vertBoneIndexes.w * constant_3)];\n"
	"	pos.x += dot(boneMatrixRowX, pos4);\n"
	"	tangent.x += dot(boneMatrixRowX.xyz, tangent3);\n"
	"	binormal.x += dot(boneMatrixRowX.xyz, binormal3);\n"
	"	normal.x += dot(boneMatrixRowX.xyz, normal3);\n"
	""
	"	boneMatrixRowY = vertBoneWeights.w * BoneArray[int(vertBoneIndexes.w * constant_3 + 1.0)];\n"
	"	pos.y += dot(boneMatrixRowY, pos4);\n"
	"	tangent.y += dot(boneMatrixRowY.xyz, tangent3);\n"
	"	binormal.y += dot(boneMatrixRowY.xyz, binormal3);\n"
	"	normal.y += dot(boneMatrixRowY.xyz, normal3);\n"
	""
	"	boneMatrixRowZ = vertBoneWeights.w * BoneArray[int(vertBoneIndexes.w * constant_3 + 2.0)];\n"
	"	pos.z += dot(boneMatrixRowZ, pos4);\n"
	"	tangent.z += dot(boneMatrixRowZ.xyz, tangent3);\n"
	"	binormal.z += dot(boneMatrixRowZ.xyz, binormal3);\n"
	"	normal.z += dot(boneMatrixRowZ.xyz, normal3);\n"
	"#endif\n"
	""
	""
	"	gl_Position = FullTransformMatrix[0] * pos.x + FullTransformMatrix[1] * pos.y + FullTransformMatrix[2] * pos.z + FullTransformMatrix[3];\n"
	"	mediump vec3 toEye = ToEyeVectorMatrix[0].xyz * pos.x + ToEyeVectorMatrix[1].xyz * pos.y + ToEyeVectorMatrix[2].xyz * pos.z + ToEyeVectorMatrix[3].xyz;\n"
	"	vrTangentAndToEyeX.xyz = normalize(TangentBinormalMatrix[0].xyz * tangent.x + TangentBinormalMatrix[1].xyz * tangent.y + TangentBinormalMatrix[2].xyz * tangent.z);\n"
	"	vrTangentAndToEyeX.w = toEye.x;\n"
	"	vrBinormalAndToEyeY.xyz = normalize(TangentBinormalMatrix[0].xyz * binormal.x + TangentBinormalMatrix[1].xyz * binormal.y + TangentBinormalMatrix[2].xyz * binormal.z);\n"
	"	vrBinormalAndToEyeY.w = toEye.y;\n"
	"	vrNormalAndToEyeZ.xyz = normalize(NormalMatrix[0].xyz * normal.x + NormalMatrix[1].xyz * normal.y + NormalMatrix[2].xyz * normal.z);\n"
	"	vrNormalAndToEyeZ.w = toEye.z;\n"
	"#ifdef TWOSIDED\n"
	"	vrNormalAndToEyeZ.xyz = dot(vrNormalAndToEyeZ.xyz, toEye) < 0.0 ? vrNormalAndToEyeZ.xyz : -vrNormalAndToEyeZ.xyz;\n"
	"#endif\n"
	""
	"#ifndef NO_LIGHTING_INFO\n"
	"vrToLightVectorXLightIntensivityAndLightIntensivity = vec4(0.0,0.0,0.0,0.0);\n"
	"vrLightColorXEnergyAndEnergy = vec4(0.0,0.0,0.0,0.0);\n"
	"vrAmbientColor = vec3(0.0,0.0,0.0);\n"
	""
	"#ifdef BUILTIN_LIGHTING\n"
	"	vrToLightVectorXLightIntensivityAndLightIntensivity += vertToLightVectorXLightIntensivityAndLightIntensivity;\n"
	"	vrLightColorXEnergyAndEnergy += vertLightColor;\n"
	"	vrAmbientColor += vertAmbientColor.xyz * vertAmbientColor.w;\n"
	"#endif\n"
	""
	"#ifdef LIGHT_PROBE_SINGLE\n"
	"	vrToLightVectorXLightIntensivityAndLightIntensivity += LightProbeSingle_probe[0];\n"
	"	vrLightColorXEnergyAndEnergy += LightProbeSingle_probe[1];\n"
	"	vrAmbientColor += LightProbeSingle_probe[2].xyz;\n"
	"#endif\n"
	""
	"#ifdef LIGHT_PROBE_STAR7\n"
	"	mediump vec3 coefs = pos * LightProbeStar7_coordMAdd[0].xyz + LightProbeStar7_coordMAdd[1].xyz;\n"
	"	mediump float k = clamp(1.0 - abs(coefs.x) - abs(coefs.y) - abs(coefs.z), 0.0, 1.0);\n"
	"	vrToLightVectorXLightIntensivityAndLightIntensivity += k * LightProbeStar7_probes[0];\n"
	"	vrLightColorXEnergyAndEnergy += k * LightProbeStar7_probes[1];\n"
	"	vrAmbientColor += k * LightProbeStar7_probes[2].xyz;\n"
	"	k = clamp(-coefs.x, 0.0, 1.0);\n"
	"	vrToLightVectorXLightIntensivityAndLightIntensivity += k * LightProbeStar7_probes[3];\n"
	"	vrLightColorXEnergyAndEnergy += k * LightProbeStar7_probes[4];\n"
	"	vrAmbientColor += k * LightProbeStar7_probes[5].xyz;\n"
	"	k = clamp(coefs.x, 0.0, 1.0);\n"
	"	vrToLightVectorXLightIntensivityAndLightIntensivity += k * LightProbeStar7_probes[6];\n"
	"	vrLightColorXEnergyAndEnergy += k * LightProbeStar7_probes[7];\n"
	"	vrAmbientColor += k * LightProbeStar7_probes[8].xyz;\n"
	"	k = clamp(-coefs.y, 0.0, 1.0);\n"
	"	vrToLightVectorXLightIntensivityAndLightIntensivity += k * LightProbeStar7_probes[9];\n"
	"	vrLightColorXEnergyAndEnergy += k * LightProbeStar7_probes[10];\n"
	"	vrAmbientColor += k * LightProbeStar7_probes[11].xyz;\n"
	"	k = clamp(coefs.y, 0.0, 1.0);\n"
	"	vrToLightVectorXLightIntensivityAndLightIntensivity += k * LightProbeStar7_probes[12];\n"
	"	vrLightColorXEnergyAndEnergy += k * LightProbeStar7_probes[13];\n"
	"	vrAmbientColor += k * LightProbeStar7_probes[14].xyz;\n"
	"	k = clamp(-coefs.z, 0.0, 1.0);\n"
	"	vrToLightVectorXLightIntensivityAndLightIntensivity += k * LightProbeStar7_probes[15];\n"
	"	vrLightColorXEnergyAndEnergy += k * LightProbeStar7_probes[16];\n"
	"	vrAmbientColor += k * LightProbeStar7_probes[17].xyz;\n"
	"	k = clamp(coefs.z, 0.0, 1.0);\n"
	"	vrToLightVectorXLightIntensivityAndLightIntensivity += k * LightProbeStar7_probes[18];\n"
	"	vrLightColorXEnergyAndEnergy += k * LightProbeStar7_probes[19];\n"
	"	vrAmbientColor += k * LightProbeStar7_probes[20].xyz;\n"
	"#endif\n"
	""
	"#ifdef LIGHT_PROBE_OCTO\n"
	"	mediump vec3 coefs = pos * LightProbeOcto_coordMAdd[0].xyz + LightProbeOcto_coordMAdd[1].xyz;\n"
	"	mediump float k = (1.0 - coefs.x) * (1.0 - coefs.y) * (1.0 - coefs.z);\n"
	"	vrToLightVectorXLightIntensivityAndLightIntensivity += k * LightProbeOcto_probes[0];\n"
	"	vrLightColorXEnergyAndEnergy += k * LightProbeOcto_probes[1];\n"
	"	vrAmbientColor += k * LightProbeOcto_probes[2].xyz;\n"
	"	k = coefs.x * (1.0 - coefs.y) * (1.0 - coefs.z);\n"
	"	vrToLightVectorXLightIntensivityAndLightIntensivity += k * LightProbeOcto_probes[3];\n"
	"	vrLightColorXEnergyAndEnergy += k * LightProbeOcto_probes[4];\n"
	"	vrAmbientColor += k * LightProbeOcto_probes[5].xyz;\n"
	"	k = (1.0 - coefs.x) * coefs.y * (1.0 - coefs.z);\n"
	"	vrToLightVectorXLightIntensivityAndLightIntensivity += k * LightProbeOcto_probes[6];\n"
	"	vrLightColorXEnergyAndEnergy += k * LightProbeOcto_probes[7];\n"
	"	vrAmbientColor += k * LightProbeOcto_probes[8].xyz;\n"
	"	k = coefs.x * coefs.y * (1.0 - coefs.z);\n"
	"	vrToLightVectorXLightIntensivityAndLightIntensivity += k * LightProbeOcto_probes[9];\n"
	"	vrLightColorXEnergyAndEnergy += k * LightProbeOcto_probes[10];\n"
	"	vrAmbientColor += k * LightProbeOcto_probes[11].xyz;\n"
	"	k = (1.0 - coefs.x) * (1.0 - coefs.y) * coefs.z;\n"
	"	vrToLightVectorXLightIntensivityAndLightIntensivity += k * LightProbeOcto_probes[12];\n"
	"	vrLightColorXEnergyAndEnergy += k * LightProbeOcto_probes[13];\n"
	"	vrAmbientColor += k * LightProbeOcto_probes[14].xyz;\n"
	"	k = coefs.x * (1.0 - coefs.y) * coefs.z;\n"
	"	vrToLightVectorXLightIntensivityAndLightIntensivity += k * LightProbeOcto_probes[15];\n"
	"	vrLightColorXEnergyAndEnergy += k * LightProbeOcto_probes[16];\n"
	"	vrAmbientColor += k * LightProbeOcto_probes[17].xyz;\n"
	"	k = (1.0 - coefs.x) * coefs.y * coefs.z;\n"
	"	vrToLightVectorXLightIntensivityAndLightIntensivity += k * LightProbeOcto_probes[18];\n"
	"	vrLightColorXEnergyAndEnergy += k * LightProbeOcto_probes[19];\n"
	"	vrAmbientColor += k * LightProbeOcto_probes[20].xyz;\n"
	"	k = coefs.x * coefs.y * coefs.z;\n"
	"	vrToLightVectorXLightIntensivityAndLightIntensivity += k * LightProbeOcto_probes[21];\n"
	"	vrLightColorXEnergyAndEnergy += k * LightProbeOcto_probes[22];\n"
	"	vrAmbientColor += k * LightProbeOcto_probes[23].xyz;\n"
	"#endif\n"
	""
	"#ifdef LIGHT_PROBE_SINGLE\n"
	"	vrToLightVectorXLightIntensivityAndLightIntensivity += LightProbeSingle_probe[0];\n"
	"	vrLightColorXEnergyAndEnergy += LightProbeSingle_probe[1];\n"
	"	vrAmbientColor += LightProbeSingle_probe[2].xyz;\n"
	"#endif\n"
	""
	"#ifdef LIGHT_PROBES\n"
	"	mediump vec4 lightVectorXLightIntensivityAndLightIntensivity = LightProbeArray[int(vertLightProbeIndexes.x * 2.0)];\n"
	"	mediump vec4 ambientColorAndLightColorPacked = LightProbeArray[int(vertLightProbeIndexes.x * 2.0 + 1.0)];\n"
	"	vrToLightVectorXLightIntensivityAndLightIntensivity += vertLightProbeWeights.x * lightVectorXLightIntensivityAndLightIntensivity;\n"
	"	mediump float energy = lightVectorXLightIntensivityAndLightIntensivity.w / dot(lightVectorXLightIntensivityAndLightIntensivity.xyz, lightVectorXLightIntensivityAndLightIntensivity.xyz);\n"
	"	vrLightColorXEnergyAndEnergy.xyz += vertLightProbeWeights.x * unpackPackedNormalizedColorF32(ambientColorAndLightColorPacked.w) * energy;\n"
	"	vrLightColorXEnergyAndEnergy.w += vertLightProbeWeights.x * energy;\n"
	"	vrAmbientColor += vertLightProbeWeights.x * ambientColorAndLightColorPacked.xyz;\n"
	""
	"	lightVectorXLightIntensivityAndLightIntensivity = LightProbeArray[int(vertLightProbeIndexes.y * 2.0)];\n"
	"	ambientColorAndLightColorPacked = LightProbeArray[int(vertLightProbeIndexes.y * 2.0 + 1.0)];\n"
	"	vrToLightVectorXLightIntensivityAndLightIntensivity += vertLightProbeWeights.y * lightVectorXLightIntensivityAndLightIntensivity;\n"
	"	energy = lightVectorXLightIntensivityAndLightIntensivity.w / dot(lightVectorXLightIntensivityAndLightIntensivity.xyz, lightVectorXLightIntensivityAndLightIntensivity.xyz);\n"
	"	vrLightColorXEnergyAndEnergy.xyz += vertLightProbeWeights.y * unpackPackedNormalizedColorF32(ambientColorAndLightColorPacked.w) * energy;\n"
	"	vrLightColorXEnergyAndEnergy.w += vertLightProbeWeights.y * energy;\n"
	"	vrAmbientColor += vertLightProbeWeights.y * ambientColorAndLightColorPacked.xyz;\n"
	""
	"	lightVectorXLightIntensivityAndLightIntensivity = LightProbeArray[int(vertLightProbeIndexes.z * 2.0 )];\n"
	"	ambientColorAndLightColorPacked = LightProbeArray[int(vertLightProbeIndexes.z * 2.0 + 1.0)];\n"
	"	vrToLightVectorXLightIntensivityAndLightIntensivity += vertLightProbeWeights.z * lightVectorXLightIntensivityAndLightIntensivity;\n"
	"	energy = lightVectorXLightIntensivityAndLightIntensivity.w / dot(lightVectorXLightIntensivityAndLightIntensivity.xyz, lightVectorXLightIntensivityAndLightIntensivity.xyz);\n"
	"	vrLightColorXEnergyAndEnergy.xyz += vertLightProbeWeights.z * unpackPackedNormalizedColorF32(ambientColorAndLightColorPacked.w) * energy;\n"
	"	vrLightColorXEnergyAndEnergy.w += vertLightProbeWeights.z * energy;\n"
	"	vrAmbientColor += vertLightProbeWeights.z * ambientColorAndLightColorPacked.xyz;\n"
	""
	"	lightVectorXLightIntensivityAndLightIntensivity = LightProbeArray[int(vertLightProbeIndexes.w * 2.0)];\n"
	"	ambientColorAndLightColorPacked = LightProbeArray[int(vertLightProbeIndexes.w * 2.0 + 1.0)];\n"
	"	vrToLightVectorXLightIntensivityAndLightIntensivity += vertLightProbeWeights.w * lightVectorXLightIntensivityAndLightIntensivity;\n"
	"	energy = lightVectorXLightIntensivityAndLightIntensivity.w / dot(lightVectorXLightIntensivityAndLightIntensivity.xyz, lightVectorXLightIntensivityAndLightIntensivity.xyz);\n"
	"	vrLightColorXEnergyAndEnergy.xyz += vertLightProbeWeights.w * unpackPackedNormalizedColorF32(ambientColorAndLightColorPacked.w) * energy;\n"
	"	vrLightColorXEnergyAndEnergy.w += vertLightProbeWeights.w * energy;\n"
	"	vrAmbientColor += vertLightProbeWeights.w * ambientColorAndLightColorPacked.xyz;\n"
	"#endif\n"
	"	vrUnocclusionOpacityOpacity2Frontlight.w = dot(vrNormalAndToEyeZ.xyz, normalize(vrToLightVectorXLightIntensivityAndLightIntensivity.xyz)) * NoLightBackFacesMultCoef + NoLightBackFacesAddCoef;\n"
	"#else\n"
	"	vrUnocclusionOpacityOpacity2Frontlight.w = dot(vrNormalAndToEyeZ.xyz, ToLightDirection) * NoLightBackFacesMultCoef + NoLightBackFacesAddCoef;\n"
	"#endif\n"
	"	"
	"}\n";

	String s;
	s << getCommonVertexShadersCode();
	s << getCommonFunctionsCode();

	switch(params.format)
	{
	case EVertexFormat_Mesh:
		break;
	case EVertexFormat_SkeletalMesh:
		s << _CS("#define BONES\n");
		break;
	case EVertexFormat_LightProbedMesh:
		s << _CS("#define LIGHT_PROBES\n");
		break;
	case EVertexFormat_BuildinLightingMesh:
		s << _CS("#define BUILTIN_LIGHTING\n");
		break;
	case EVertexFormat_HeightMap:
		s << _CS("#define HEIGHTMAP\n");
		break;
	}

	switch(params.lightProbe)
	{
	case ELightProbeType_Single:
		if(params.format == EVertexFormat_BuildinLightingMesh)
			s << _CS("#define LIGHT_PROBE_SINGLE\n");
		else
			s << _CS("#define NO_LIGHTING_INFO\n");
		break;
	case ELightProbeType_Star7:
		s << _CS("#define LIGHT_PROBE_STAR7\n");
		break;
	case ELightProbeType_Octo:
		s << _CS("#define LIGHT_PROBE_OCTO\n");
		break;
	}

	if(params.twoSided)
		s << _CS("#define TWOSIDED\n");

	s << constString(str);
	return s;
}

Array<String> RightDrawer::getVertexShaderAttributesCode(EVertexFormat fmt)
{
	Array<String> attr;

	if(fmt == EVertexFormat_HeightMap)
	{
		attr.push(String(_CS("vertPositionXY")));
		attr.push(String(_CS("vertHeight")));
		attr.push(String(_CS("vertDerivata")));
		attr.push(String(_CS("vertLayers")));
	}

	if(fmt == EVertexFormat_Mesh || fmt == EVertexFormat_BuildinLightingMesh || fmt == EVertexFormat_SkeletalMesh || fmt == EVertexFormat_LightProbedMesh )
	{
		attr.push(String(_CS("vertPosition")));
		attr.push(String(_CS("vertTexCoord")));
		attr.push(String(_CS("vertTangentAndOpacity")));
		attr.push(String(_CS("vertBinormalAndUnocclusion")));
		attr.push(String(_CS("vertNormal")));

		switch(fmt)
		{
		case EVertexFormat_BuildinLightingMesh:
			attr.push(String(_CS("vertToLightVectorXLightIntensivityAndLightIntensivity")));
			attr.push(String(_CS("vertLightColor")));
			attr.push(String(_CS("vertAmbientColor")));
			break;
		case EVertexFormat_SkeletalMesh:
			attr.push(String(_CS("vertBoneIndexes")));
			attr.push(String(_CS("vertBoneWeights")));
			break;
		case EVertexFormat_LightProbedMesh:
			attr.push(String(_CS("vertLightProbeIndexes")));
			attr.push(String(_CS("vertLightProbeWeights")));
			break;
		}
	}

	return attr;
}


String RightDrawer::getMeshPixelShaderCode(MeshPixelShaderIndex params)
{
	const char* str =
	"varying highp vec2 vrTexCoord;\n"
	"varying mediump vec4 vrTangentAndToEyeX;\n"
	"varying mediump vec4 vrBinormalAndToEyeY;\n"
	"varying mediump vec4 vrNormalAndToEyeZ;\n"
	"varying mediump vec4 vrUnocclusionOpacityOpacity2Frontlight;\n"
	"#ifndef NO_LIGHTING_INFO\n"
	"	varying mediump vec4 vrToLightVectorXLightIntensivityAndLightIntensivity;\n"
	"	varying mediump vec4 vrLightColorXEnergyAndEnergy;\n"
	"	varying mediump vec3 vrAmbientColor;\n"
	"#else\n"
	"	uniform mediump vec3 ToLightDirection;\n"
	"	uniform mediump vec3 LightColor;\n"
	"	uniform mediump vec3 AmbientColor;\n"
	"#endif\n"
	""
	"#ifdef ENV_CUBE\n"
	"	uniform samplerCube EnvTex;\n"
	"	uniform mediump vec3 EnvColor;\n"
	"#endif\n"
	"uniform sampler2D Tex0;\n"
	"uniform sampler2D Tex1;\n"
	"uniform mediump float NormalMapScale;\n"
	"uniform mediump float OpacityScale;\n"
	"uniform mediump float GlossinesScale;\n"
	"uniform mediump float SpecularMaskScale;\n"
	"uniform mediump float GlossinesDeltha;\n"
	"uniform mediump float SpecularMaskDeltha;\n"
	"uniform mediump float Frensel;\n"
	"uniform mediump float Metal;\n"
	"uniform mediump vec4 ColorMatrix[4];\n"
	"#ifdef DUAL\n"
	"	uniform sampler2D Tex02;\n"
	"	uniform sampler2D Tex12;\n"
	"	uniform mediump float NormalMapScale2;\n"
	"	uniform mediump float OpacityScale2;\n"
	"	uniform mediump float GlossinesScale2;\n"
	"	uniform mediump float SpecularMaskScale2;\n"
	"	uniform mediump float GlossinesDeltha2;\n"
	"	uniform mediump float SpecularMaskDeltha2;\n"
	"	uniform mediump float Frensel2;\n"
	"	uniform mediump float Metal2;\n"
	"	uniform mediump vec4 ColorMatrix2[4];\n"
	"#endif\n"
	"uniform mediump float ColorMultiplyer;\n"
	"uniform mediump float RoughtnessSqrExp2Coef;\n"
	"#ifndef BACKLIGHT\n"
	"	uniform mediump float BacklightMultCoef;\n"
	"	uniform mediump float BacklightAddCoef;\n"
	"#endif\n"
	"void main()\n"
	"{\n"
	"	const mediump float constant_epsilon = 0.001;\n"
	""
	"	mediump vec4 tex0 = texture2D(Tex0, vrTexCoord.xy);\n"
	"	mediump vec4 tex1 = texture2D(Tex1, vrTexCoord.xy);\n"
	"	mediump vec3 albedo = ColorMatrix[0].rgb * tex0.x + ColorMatrix[1].rgb * tex0.y + ColorMatrix[2].rgb * tex0.z + ColorMatrix[3].rgb;\n"
	"	mediump vec2 normalXY = (vec2(tex0.w, tex1.w) * 2.0 - 1.0) * NormalMapScale;\n"
	"	mediump float specularMask = tex1.r * SpecularMaskScale + SpecularMaskDeltha;\n"
	"	mediump float glossines = tex1.g * GlossinesScale + GlossinesDeltha;\n"
	"#ifdef OPACITY\n"
	"#ifdef ALPHA_TEST\n"
	"	if(tex1.b < 0.5) discard;\n"
	"	mediump float opacity = OpacityScale;\n"
	"#else\n"
	"	mediump float opacity = tex1.b * OpacityScale;\n"
	"#endif\n"
	"	mediump float metal = Metal;\n"
	"#else\n"
	"	mediump float opacity = OpacityScale;\n"
	"	mediump float metal = tex1.b * Metal;\n"
	"#endif\n"
	"	mediump float frenselBase = Frensel;\n"
	"\n"
	"#ifdef DUAL\n"
	"	mediump float layerCoef = clamp(vrUnocclusionOpacityOpacity2Frontlight.y / (vrUnocclusionOpacityOpacity2Frontlight.y + vrUnocclusionOpacityOpacity2Frontlight.z + constant_epsilon), 0.0, 1.0);\n"
	"	albedo *= layerCoef;\n"
	"	normalXY *= layerCoef;\n"
	"	specularMask *= layerCoef;\n"
	"	glossines *= layerCoef;\n"
	"	opacity *= layerCoef;\n"
	"	metal *= layerCoef;\n"
	"	frenselBase *= layerCoef;\n"
	"	mediump float layer2Coef = 1.0 - layerCoef;\n"
	"	tex0 = texture2D(Tex02, vrTexCoord.xy);\n"
	"	tex1 = texture2D(Tex12, vrTexCoord.xy);\n"
	"	albedo += (ColorMatrix2[0].rgb * tex0.x + ColorMatrix2[1].rgb * tex0.y + ColorMatrix2[2].rgb * tex0.z + ColorMatrix2[3].rgb) * layer2Coef;\n"
	"	normalXY += (vec2(tex0.w, tex1.w) * 2.0 - 1.0) * (NormalMapScale2 * layer2Coef);\n"
	"	specularMask += (tex1.r * SpecularMaskScale2 + SpecularMaskDeltha2) * layer2Coef;\n"
	"	glossines += (tex1.g * GlossinesScale2 + GlossinesDeltha2) * layer2Coef;\n"
	"#ifdef OPACITY\n"
	"	opacity += tex1.b * OpacityScale2 * layer2Coef;\n"
	"	metal += Metal2 * layer2Coef;\n"
	"#else\n"
	"	opacity += OpacityScale2 * layer2Coef;\n"
	"	metal += tex1.b * Metal2 * layer2Coef;\n"
	"#endif\n"
	"	frenselBase += Frensel2 * layer2Coef;\n"
	"#endif\n"
	"\n"
	"#ifdef BACKLIGHT\n"
	"	mediump float backlightCoef = clamp(2.0 - opacity, 0.0, 1.0);\n"
	"#endif\n"
	"#ifdef DUAL\n"
	"	opacity = clamp(opacity * (vrUnocclusionOpacityOpacity2Frontlight.y + vrUnocclusionOpacityOpacity2Frontlight.z), 0.0, 1.0);\n"
	"#else\n"
	"	opacity = clamp(opacity * vrUnocclusionOpacityOpacity2Frontlight.y, 0.0, 1.0);\n"
	"#endif\n"
	"	metal = clamp(metal, 0.0, 1.0);\n"
	"\n"
	"	mediump vec3 normal = vrTangentAndToEyeX.xyz * normalXY.x + vrBinormalAndToEyeY.xyz * normalXY.y + vrNormalAndToEyeZ.xyz * sqrt(max(0.0, 1.0 - dot(normalXY, normalXY)));\n"
	"	normal = normalize(normal);\n"
	"\n"
	"	albedo *= albedo;\n"
	"	specularMask *= specularMask;\n"
	"\n"
	"//light data\n"
	"	mediump float lightingCoef = clamp(vrUnocclusionOpacityOpacity2Frontlight.w, 0.0, 1.0);\n"
	"	lightingCoef *= lightingCoef;\n"
	"#ifndef NO_LIGHTING_INFO\n"
	"	mediump float lightVectorSqr = dot(vrToLightVectorXLightIntensivityAndLightIntensivity, vrToLightVectorXLightIntensivityAndLightIntensivity) + constant_epsilon;\n"
	"	mediump float lightEnergy = vrToLightVectorXLightIntensivityAndLightIntensivity.w * vrToLightVectorXLightIntensivityAndLightIntensivity.w * vrToLightVectorXLightIntensivityAndLightIntensivity.w / lightVectorSqr;\n"
	"	mediump vec3 toLightDirection = inversesqrt(lightVectorSqr) * vrToLightVectorXLightIntensivityAndLightIntensivity.xyz;\n"
	"	mediump vec3 lightColor = vrLightColorXEnergyAndEnergy.xyz * (lightEnergy * lightingCoef / vrLightColorXEnergyAndEnergy.w);\n"
	"	mediump vec3 ambientColor = vrAmbientColor;\n"
	"#else\n"
	"	mediump vec3 lightColor = LightColor * lightingCoef;\n"
	"	mediump vec3 toLightDirection = ToLightDirection;\n"
	"	mediump vec3 ambientColor = AmbientColor;\n"
	"#endif\n"
	"\n"
	"//diffuse lighting\n"
	"#ifdef ANISOTROPIC\n"
	"	mediump float diffuse = max(0.0, 1.0 - abs(dot(toLightDirection, normal)));\n"
	"#else\n"
	"	mediump float diffuse = max(0.0, dot(toLightDirection, normal));\n"
	"#endif\n"
	"#ifdef BACKLIGHT\n"
	"	diffuse += backlightCoef;\n"
	"#endif\n"
	"	mediump vec3 color = albedo * (lightColor * diffuse + ambientColor);\n"
	"\n"
	"#ifndef FAST\n"
	"//specular lighting\n"
	"	mediump vec3 eye = vec3(vrTangentAndToEyeX.w, vrBinormalAndToEyeY.w, vrNormalAndToEyeZ.w);\n"
	"	eye = normalize(eye);\n"
	"	mediump float NdotE = max(0.0, dot(eye, normal));\n"
	"	mediump float OneMinusNdotE = 1.0 - NdotE;\n"
	"	mediump float frensel = frenselBase + (1.0 - frenselBase) * OneMinusNdotE * OneMinusNdotE * OneMinusNdotE * OneMinusNdotE * OneMinusNdotE;\n"
	"	mediump float reflectionCoef = frensel * specularMask;\n"
	"	mediump float roughtnessSqr = exp2(RoughtnessSqrExp2Coef * glossines);\n"
	"	mediump float cosAlpha2 = dot(normalize(toLightDirection + eye), normal) + constant_epsilon;\n"
	"	cosAlpha2 *= cosAlpha2;\n"
	"	mediump float specular = reflectionCoef * exp( ( cosAlpha2 - 1.0 ) / ( cosAlpha2 * roughtnessSqr )  ) / ( roughtnessSqr * cosAlpha2 * cosAlpha2 );\n"
	"	color += specular * lightColor * (metal * albedo + (1.0 - metal));\n"
	""
	"#ifdef ENV_CUBE\n"
	"	mediump vec3 reflectDir = - (eye - normal * (2.0 * dot(normal, eye)));\n"
	"	mediump vec3 env = textureCube(EnvTex, reflectDir).xyz * EnvColor;\n"
	"	color += env * env * (reflectionCoef * vrUnocclusionOpacityOpacity2Frontlight.x);\n"
	"#endif\n"
	"#endif\n"
	"	color *= ColorMultiplyer;\n"
	"#ifndef FAST\n"
	"	color.r = sqrt(color.r);\n"
	"	color.g = sqrt(color.g);\n"
	"	color.b = sqrt(color.b);\n"
	"#else\n"
	"	mediump vec3 c = vec3(0.5,0.5,0.5);\n"
	"	c = 0.5 * (c + color / c);\n"
	"	c = 0.5 * (c + color / c);\n"
	"	color = 0.5 * (c + color / c);\n"
	"#endif\n"
	"	gl_FragColor.rgb = color * opacity;\n"
	"	gl_FragColor.a = opacity;\n"
	"//	gl_FragColor = vec4(normalize(vrNormalAndToEyeZ.xyz) * 0.5 + 0.5, 0.0);\n"
	"}\n";
	
	String s;
	//if(fast)
	//	s << _CS("#define FAST\n");
	if(params.dualLayer)
		s << _CS("#define DUAL\n");
	if(params.singleLightProbe)
		s << _CS("#define NO_LIGHTING_INFO\n");
	if(params.envCubeTex)
		s << _CS("#define ENV_CUBE\n");
	if(params.anisotropic)
		s << _CS("#define ANISOTROPIC\n");
	if(params.backlight)
		s << _CS("#define BACKLIGHT\n");
	if(params.opacity)
		s << _CS("#define OPACITY\n");
	if(params.alphaTest)
		s << _CS("#define ALPHA_TEST\n");
	s << getCommonPixelShadersCode();
	s << constString(str);
	//s << constString("void main() { gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); }");
	return s;
}

String RightDrawer::getWaterPlanePixelShaderCode(WaterPlanePixelShaderIndex params)
{
	const char* str =
	"varying highp vec2 vrTexCoord;\n"
	"varying mediump vec4 vrTangentAndToEyeX;\n"
	"varying mediump vec4 vrBinormalAndToEyeY;\n"
	"varying mediump vec4 vrNormalAndToEyeZ;\n"
	"varying mediump vec4 vrUnocclusionOpacityOpacity2Frontlight;\n"
	"#ifndef NO_LIGHTING_INFO\n"
	"	varying mediump vec4 vrToLightVectorXLightIntensivityAndLightIntensivity;\n"
	"	varying mediump vec4 vrLightColorXEnergyAndEnergy;\n"
	"	varying mediump vec3 vrAmbientColor;\n"
	"#else\n"
	"	uniform mediump vec3 ToLightDirection;\n"
	"	uniform mediump vec3 LightColor;\n"
	"	uniform mediump vec3 AmbientColor;\n"
	"#endif\n"
	""
	"uniform sampler2D EnvTex;\n"
	"uniform mediump vec3 EnvColor;\n"
	"uniform sampler2D Tex;\n"
	"uniform mediump vec3 BlendCoefs;\n"
	"uniform mediump float OpacityScale;\n"
	"uniform mediump vec3 Albedo;\n"
	"uniform mediump float SpecularMask;\n"
	"uniform mediump float Frensel;\n"
	"uniform mediump float RoughtnessSqr;\n"
	"uniform mediump float EnvCoordScale;\n"
	"uniform mediump float EnvTexCoordScale;\n"
	"uniform mediump float ColorMultiplyer;\n"
	""
	"void main()\n"
	"{\n"
	"	const mediump float constant_epsilon = 0.001;\n"
	""
	"	mediump vec4 tex = texture2D(Tex, vrTexCoord);\n"
	"	mediump vec2 normalXY = tex.xy * BlendCoefs.x + tex.zw * BlendCoefs.y + BlendCoefs.z;\n"
	"	normalXY *= inversesqrt(1.0 - normalXY.x*normalXY.x - normalXY.y * normalXY.y);\n"
	"	mediump vec3 normal = vrTangentAndToEyeX.xyz * normalXY.x + vrBinormalAndToEyeY.xyz * normalXY.y + vrNormalAndToEyeZ.xyz;\n"
	"	normal = normalize(normal);\n"
	"	mediump float opacity = OpacityScale * vrUnocclusionOpacityOpacity2Frontlight.y;\n"
	"//light data\n"
	"	mediump float lightingCoef = clamp(vrUnocclusionOpacityOpacity2Frontlight.w, 0.0, 1.0);\n"
	"	lightingCoef *= lightingCoef;\n"
	"#ifndef NO_LIGHTING_INFO\n"
	"	mediump float lightVectorSqr = dot(vrToLightVectorXLightIntensivityAndLightIntensivity, vrToLightVectorXLightIntensivityAndLightIntensivity) + constant_epsilon;\n"
	"	mediump float lightEnergy = vrToLightVectorXLightIntensivityAndLightIntensivity.w * vrToLightVectorXLightIntensivityAndLightIntensivity.w * vrToLightVectorXLightIntensivityAndLightIntensivity.w / lightVectorSqr;\n"
	"	mediump vec3 toLightDirection = inversesqrt(lightVectorSqr) * vrToLightVectorXLightIntensivityAndLightIntensivity.xyz;\n"
	"	mediump vec3 lightColor = vrLightColorXEnergyAndEnergy.xyz * (lightEnergy * lightingCoef / vrLightColorXEnergyAndEnergy.w);\n"
	"	mediump vec3 ambientColor = vrAmbientColor;\n"
	"#else\n"
	"	mediump vec3 lightColor = LightColor * lightingCoef;\n"
	"	mediump vec3 toLightDirection = ToLightDirection;\n"
	"	mediump vec3 ambientColor = AmbientColor;\n"
	"#endif\n"
	"\n"
	"//diffuse lighting\n"
	"	mediump float diffuse = max(0.0, dot(toLightDirection, normal));\n"
	"	mediump vec3 color = Albedo * (lightColor * diffuse + ambientColor);\n"
	"\n"
	"	mediump vec3 eye = vec3(vrTangentAndToEyeX.w, vrBinormalAndToEyeY.w, vrNormalAndToEyeZ.w);\n"
	"	eye = normalize(eye);\n"
	"#ifndef FAST\n"
	"//specular lighting\n"
	"	mediump float NdotE = max(0.0, dot(eye, normal));\n"
	"	mediump float OneMinusNdotE = 1.0 - NdotE;\n"
	"	mediump float frensel = Frensel + (1.0 - Frensel) * OneMinusNdotE * OneMinusNdotE * OneMinusNdotE * OneMinusNdotE * OneMinusNdotE;\n"
	"	mediump float reflectionCoef = frensel * SpecularMask;\n"
	"	mediump float cosAlpha2 = dot(normalize(toLightDirection + eye), normal) + constant_epsilon;\n"
	"	cosAlpha2 *= cosAlpha2;\n"
	"	mediump float specular = reflectionCoef * exp( ( cosAlpha2 - 1.0 ) / ( cosAlpha2 * RoughtnessSqr )  ) / ( RoughtnessSqr * cosAlpha2 * cosAlpha2 );\n"
	"	color += specular * lightColor;\n"
	"#else\n"
	"	mediump float reflectionCoef = Frensel * SpecularMask;\n"
	"#endif\n"
	""
	"	mediump vec3 reflectDir = - (eye - normal * (2.0 * dot(normal, eye)));\n"
	"	mediump vec3 env = texture2D(EnvTex, reflectDir.xy * (EnvCoordScale / reflectDir.z) + vrTexCoord.xy * EnvTexCoordScale).xyz * EnvColor;\n"
	"	color += env * env * (reflectionCoef * vrUnocclusionOpacityOpacity2Frontlight.x);\n"
	""
	"	color *= ColorMultiplyer;\n"
	"#ifndef FAST\n"
	"	color.r = sqrt(color.r);\n"
	"	color.g = sqrt(color.g);\n"
	"	color.b = sqrt(color.b);\n"
	"#else\n"
	"	mediump vec3 c = vec3(0.5,0.5,0.5);\n"
	"	c = 0.5 * (c + color / c);\n"
	"	c = 0.5 * (c + color / c);\n"
	"	color = 0.5 * (c + color / c);\n"
	"#endif\n"
	"	gl_FragColor.rgb = color * opacity;\n"
	"	gl_FragColor.a = opacity;\n"
	"	//gl_FragColor = vec4(normal * 0.5 + 0.5, 0.0);\n"
	"}\n";

	String s;
	//if(fast)
	//	s << _CS("#define FAST\n");
	if(params.singleLightProbe)
		s << _CS("#define NO_LIGHTING_INFO\n");
	s << getCommonPixelShadersCode();
	s << constString(str);
	return s;
}

} 