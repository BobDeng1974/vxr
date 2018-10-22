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

#include "../../include/core/scene.h"

namespace vxr
{

  Scene::Scene()
  {
    set_name("Scene");
    root_.alloc()->set_name("Scene Root");
    root_->scene_id_ = id();
  }

  Scene::~Scene()
  {

  }

  void Scene::addObject(ref_ptr<GameObject> obj)
  {
    obj->transform()->set_parent(root_->transform());
    if (!default_camera_)
    {
      default_camera_ = obj->getComponent<Camera>();
    }
  }

  void Scene::set_default_camera(ref_ptr<Camera> cam)
  {
    default_camera_ = cam;
  }

  ref_ptr<Camera> Scene::default_camera()
  {
    return default_camera_;
  }

  ref_ptr<GameObject> Scene::root()
  {
    return root_;
  }

}