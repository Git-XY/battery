from building import *
Import('rtconfig')

src   = []
cwd   = GetCurrentDir()

# add battery src files.
if GetDepend('PKG_USING_BATTERY'):
    src += Glob('src/battery.c')

# add battery include path.
path  = [cwd + '/inc']

# add src and include to group.
group = DefineGroup('battery', src, depend = ['PKG_USING_BATTERY'], CPPPATH = path)
Return('group')