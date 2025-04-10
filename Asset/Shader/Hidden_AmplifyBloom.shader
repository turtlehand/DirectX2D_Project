//////////////////////////////////////////
//
// NOTE: This is *not* a valid shader file
//
///////////////////////////////////////////
Shader "Hidden/AmplifyBloom" {
Properties {
_MainTex (" ", 2D) = "black" { }
_AnamorphicRTS0 (" ", 2D) = "black" { }
_AnamorphicRTS1 (" ", 2D) = "black" { }
_AnamorphicRTS2 (" ", 2D) = "black" { }
_AnamorphicRTS3 (" ", 2D) = "black" { }
_AnamorphicRTS4 (" ", 2D) = "black" { }
_AnamorphicRTS5 (" ", 2D) = "black" { }
_AnamorphicRTS6 (" ", 2D) = "black" { }
_AnamorphicRTS7 (" ", 2D) = "black" { }
_LensFlareLUT (" ", 2D) = "black" { }
}
SubShader {
 Pass {
  Name "frag_threshold"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 63897
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_thresholdMask"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 89948
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_anamorphicGlare"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 160591
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_lensFlare0"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 222232
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_lensFlare1"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 309675
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_lensFlare2"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 370052
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_lensFlare3"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 453897
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_lensFlare4"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 506839
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_lensFlare5"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 559949
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_downsamplerNoWeightedAvg"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 651075
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_downsampler_with_karis"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 677097
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_downsampler_without_karis"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 774185
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_downsampler_temp_filter_with_karis"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 849705
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_downsampler_temp_filter_without_karis"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 895189
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_horizontal_gaussian_blur"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 927839
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_vertical_gaussian_blur"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 1007810
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_vertical_gaussian_blur_temp_filter"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 1075355
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_upscaleTentFirstPass"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 1157886
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_upscaleTent"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 1184029
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_weightedAddPS1"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 1290203
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_weightedAddPS2"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 1342618
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_weightedAddPS3"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 1403201
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_weightedAddPS4"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 1443195
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_weightedAddPS5"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 1543796
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_weightedAddPS6"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 1606474
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_weightedAddPS7"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 1698184
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_weightedAddPS8"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 1713268
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_BokehFiltering"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 1781751
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_BokehComposition2S"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 1887392
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_BokehComposition3S"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 1962584
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_BokehComposition4S"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 1972178
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_BokehComposition5S"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 2093301
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_BokehComposition6S"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 2129582
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
 Pass {
  Name "frag_decode"
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 2190352
Program "vp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
Program "fp" {
SubProgram "d3d11 " {
"// shader disassembly not supported on DXBC"
}
SubProgram "d3d11 " {
Keywords { "AB_HIGH_PRECISION" }
"// shader disassembly not supported on DXBC"
}
}
}
}
}