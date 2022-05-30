project('madshader', ['cpp'], default_options: ['cpp_std=c++17', 'buildtype=debug'])

os_system = target_machine.system()
cflags = []

if os_system == 'darwin'
  cflags += '-DGL_SILENCE_DEPRECATION=1'
  cflags += '-DOS_DARWIN=1'
endif

gl = dependency('OpenGL', required:true)
glm = dependency('glm', required: true)
glew = dependency('glew', required: true)
glfw = dependency('glfw3', required:true)

madshader_srcs = files('main.cpp')
madshader_incs = include_directories('.')
executable('madshader', madshader_srcs,
           include_directories: [madshader_incs],
           cpp_args: cflags,
           dependencies: [gl, glew, glfw])
