#pragma once

// ----------------------------------------------------------------------------------------
// MIT License
// 
// Copyright(c) 2018 V�ctor �vila
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// ----------------------------------------------------------------------------------------

#include "../graphics/render_context.h"

/**
* \file shader.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Shader Uniform Structures. 
*
*/
namespace vxr
{

  namespace Shader
  {

    static const char* u_model = "u_model";

    struct CommonData
    {
      mat4 u_proj;
      mat4 u_view;

      vec2 resolution;
      vec2 xy;

      vec4 u_clear_color;
    };

    struct LightData
    {

    };

    struct Data
    {
      const char* name = "Uniforms";
      Usage::Enum usage = Usage::Stream;
      struct Uniforms
      {
        union Specific 
        {
          // Add here specific uniform structures for any new shaders.
          struct Standard
          {
            vec4 color;
          } std;
          struct Planet
          {
            vec2 elevationMinMax;
          } planet;
        } specific;
      } u;
    };

    string load(const char* file);

  }

} /* end of vxr namespace */