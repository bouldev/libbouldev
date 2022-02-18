project('bouldev', 'c', 'cpp', version: '0.0.1')
project_description = 'Bouldev Open User Launcher Device Environment Verifier'

project_headers = [
	'src/common/libbouldev_private.h'
]

project_source_files = [
	'src/common/run_cmd.c',
	'src/common/path_utils.c',
	'src/vm/chroot_detect.c',
	'src/vm/vm_detect.cpp'
]

build_args = [
	'-DPROJECT_NAME=' + meson.project_name(),
	'-DPROJECT_VERSION=' + meson.project_version(),
]

build_args += [
	'-DBUILDING_LIBBOULDEV',
]

add_global_arguments(build_args, language: 'c')
add_global_arguments(build_args, language: 'cpp')
project_target = shared_library(
  meson.project_name(),
  project_source_files,
  install : true,
  c_args : build_args,
  include_directories : include_directories('src/common'),
)

install_headers(project_headers)

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name : meson.project_name(),
  filebase : meson.project_name(),
  description : project_description,
  subdirs : meson.project_name(),
  libraries : project_target,
)
