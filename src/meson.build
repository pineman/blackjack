project('blackjack', 'c')

cc = meson.get_compiler('c')
math = cc.find_library('m', required: false)
sdl = dependency('sdl2')
sdl_ttf = dependency('SDL2_ttf')
sdl_image = dependency('SDL2_image')

executable('blackjack',
	sources: ['ea.c', 'error.c', 'file.c', 'list.c', 'logic.c', 'main.c', 'sdl.c'],
	dependencies: [math, sdl, sdl_ttf, sdl_image])
