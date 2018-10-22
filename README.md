[![vxr engine logo](/assets/textures/readme/logo.png)](https://avilapa.github.io)

# What is [vxr](https://github.com/avilapa/vxr)?

__vxr__ is a project that emerged from the desire to build a __general purpose rendering tool__ that I could use to implement anything I was curious about or wanted to learn about.

As it stands now, it is a __multithreaded 3D game development engine written in C++__ and oriented to support different rendering backends. The engine has two different APIs for development:

  - A __lower level API__ based on render display list commands that can be used to build __small but efficient__ applications.
  - A __higher level API__ built on top of the lower level one, which abstracts it to provide a __rendering agnostic and component oriented__ API.

![06-PlanetEditor](/assets/textures/readme/06-PlanetEditor.png)

# Building

This project uses [GENie.lua](https://github.com/bkaradzic/GENie) to create the project solution. From for example [babun](http://babun.github.io/) you will have to execute the following commands:

	cd project
	../tools/genie.exe [parameters] <vs2017|vs2015>
  
### Parameters

 - __--gl__: Build with OpenGL backend.
 - __--debug_tracing__: Build with minitrace and enable _trace.json_ file creation.
 - __--no-console__: Do not launch a console with the graphic window.
 - __--no-threading__: Launch rendering on the same thread as the logic.
 - __--no-ui__: Disable all the UI (will still compile ImGui though).

# Examples

A list of examples are provided within the solution to showcase the engine's capabilities. To create a new example, add a call to the function '__makeProject("yourExampleName");__' at the end of the file _project/genie.lua_ and build.

#### 01-HelloWorld
![01-HelloWorld](/assets/textures/readme/01-HelloWorld.png)
#### 02-Instancing
![02-Instancing](/assets/textures/readme/02-Instancing.png)
#### 03-Framebuffers
![03-Framebuffers](/assets/textures/readme/03-Framebuffers.png)
#### 04-Mesh
![04-Mesh](/assets/textures/readme/04-Mesh.png)
#### 06-PlanetEditor
![06-PlanetEditor](/assets/textures/readme/06-PlanetEditor.png)
# Features

- Multithreaded agnostical graphics API
- _GameObjects_ with the following components currently implemented:
  - Transform
  - Mesh Filter
  - Renderer
  - Camera
  - Custom Component
- Model loading (_.obj_)

# API Reference

### Lower Level API

```c++
VXR_DEFINE_APP_MAIN(vxr::Main)

#define GLSL(...) "#version 330\n" #__VA_ARGS__

namespace vxr 
{
  static float vertex_data[] = {
   -0.5, -0.5, 0.0,  1.0,  0.0,  0.0,
    0.5, -0.5, 0.0,  0.0,  1.0,  0.0,
    0.0,  0.5, 0.0,  0.0,  0.0,  1.0,
  };

  static vxr::uint16 index_data[] = {
    0, 1, 2,
  };

  void Main::start()
  {
    window_title_ = (char*)malloc(512 + strlen("VXR Hello World Test"));

    vertex_buffer_ = Engine::ref().gpu()->createBuffer({ BufferType::Vertex, 
                                                         sizeof(vertex_data), 
                                                         Usage::Static });
    index_buffer_  = Engine::ref().gpu()->createBuffer({ BufferType::Index,  
                                                         sizeof(index_data),  
                                                         Usage::Static });

    gpu::Material::Info mat_info;
    mat_info.shader.vert = GLSL(
      in vec3 a_position;
      in vec3 a_color;
      out vec3 color;
      void main() { gl_Position = vec4(a_position, 1.0); color = a_color; }
    );
    mat_info.shader.frag = GLSL(
      in vec3 color;
      out vec4 fragColor;
      void main() { fragColor = vec4(color, 1.0); }
    );

    mat_info.attribs[0] = { "a_position", VertexFormat::Float3 };
    mat_info.attribs[1] = { "a_color",    VertexFormat::Float3 };

    material_ = Engine::ref().gpu()->createMaterial(mat_info);

    DisplayList frame;
    frame.fillBufferCommand()
      .set_buffer(vertex_buffer_)
      .set_data(vertex_data)
      .set_size(sizeof(vertex_data));
    frame.fillBufferCommand()
      .set_buffer(index_buffer_)
      .set_data(index_data)
      .set_size(sizeof(index_data));
    Engine::ref().submitDisplayList(std::move(frame));
  }

  void Main::renderUpdate()
  {
    sprintf(window_title_,
      "%s: %d FPS @ 1280 x 720, Rendering time: %f ms",
      "VXR Hello World Test",
      fps(), 1.0 / (double)fps());

    Engine::ref().window()->set_title(window_title_);
    
    DisplayList frame;
    frame.clearCommand()
      .set_color(Color::Black);
    frame.setupMaterialCommand()
      .set_material(material_)
      .set_buffer(0, vertex_buffer_);
    frame.renderCommand()
      .set_index_buffer(index_buffer_)
      .set_count(sizeof(index_data) / sizeof(uint16))
      .set_type(IndexFormat::UInt16);
    Engine::ref().submitDisplayList(std::move(frame));

    Application::renderUpdate();
  }

} /* end of vxr namespace */
```

### Higher Level API

```c++
VXR_DEFINE_APP_MAIN(vxr::Main)

namespace vxr
{

  Main::Main()
  {
    window_title_ = (char*)malloc(512 + strlen("VXR Instancing Test"));
    Params p;
    p.gpu = { 100, 100, 100, 100 };
    p.window = { { 1920, 1080} };
    Engine::ref().set_preinit_params(p);
  }

  void Main::init()
  {
    Application::init();
  }

  void Main::start()
  {
    ref_ptr<Scene> scene_;
    scene_.alloc();
    Engine::ref().loadScene(scene_);

    cam_.alloc()->set_name("Camera");
    cam_->addComponent<Camera>()->set_background_color(Color(0.0f, 0.125f, 0.3f, 1.0f));
    cam_->transform()->set_local_position(vec3(0,0,4));
    scene_->addObject(cam_);

    mesh_ = Asset::loadModelOBJ("../../assets/meshes/obj/teapot_mesh.obj");
    mesh_->set_name("Teapot");
    mesh_->transform()->set_local_rotation(vec3(glm::radians(90.0f), 0, 0));
    scene_->addObject(mesh_);

    Application::start();
  }

  void Main::update()
  {
    mesh_->transform()->set_local_rotation(mesh_->transform()->local_rotation() 
					   + vec3(0.21, 0.12, 0.5) * deltaTime());
    Application::update();
  }

  void Main::renderUpdate()
  {
    updateWindowTitle();

    Engine::ref().submitUIFunction([this]() { ui::Editor(); });

    Application::renderUpdate();
  }

  void Main::stop()
  {
    Application::stop();
  }

  void Main::updateWindowTitle()
  {
    sprintf(window_title_,
      "%s: %d FPS @ 1920 x 1080",
      "VXR Mesh Example",
      fps());

    Engine::ref().window()->set_title(window_title_);
  }

} /* end of vxr namespace */
```

# Supported Platforms

- Windows

# Supported Backends

- OpenGL
- _WIP: DirectX3D 11_

# Dependencies

The project includes some 3rd party dependencies, but not all of them are necessary (e.g. you won't need GLEW if you generate the project to use DirectX3D 11, you won't need minitrace if you don't want trace debugging, etc).

- [glm](https://github.com/g-truc/glm)
- [imgui](https://github.com/ocornut/imgui)
- [tinyobjloader](https://github.com/syoyo/tinyobjloader)
- [voxelizer](https://github.com/karimnaaji/voxelizer)
- [stb](https://github.com/nothings/stb)

### Optional Dependencies

- [glew](http://glew.sourceforge.net/) - _If you are building with OpenGL (--gl)_
- [glfw](https://github.com/glfw/glfw) - _If you are building with OpenGL (--gl)_
- [minitrace](https://github.com/hrydgard/minitrace) - _If you are building with debug tracing (--debug-tracing)_

# Thanks

Special thanks to my previous tutor [PpluX](https://github.com/pplux), top coder who lately wrote [px_render.h](https://github.com/pplux/px/blob/master/px_render.h), from which I learned a lot and incorporated part of it into my OpenGL backend.

# [MIT License](/LICENSE)

