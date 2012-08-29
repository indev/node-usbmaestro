import os

def set_options(opt):
  opt.tool_options("compiler_cxx")
  opt.tool_options("compiler_cc")

def configure(conf):
  conf.check_tool('compiler_cxx')
  conf.check_tool("compiler_cc")	
  conf.check_tool('node_addon')
  conf.check_cfg(package='libusb-1.0', uselib_store='USB10', mandatory=1, args='--cflags --libs')  
  conf.env.append_unique('CPPFLAGS', ["-D_FILE_OFFSET_BITS=64", "-D_LARGEFILE_SOURCE"])
  conf.env.append_unique('CXXFLAGS', ["-Wall"])

def build(bld):
  obj = bld.new_task_gen("cxx", "shlib", "node_addon")
  obj.target = "usbmaestro"
  obj.source = ["src/node_main.cpp", "src/UsbDevice.cpp"]
  obj.uselib = ["USB10"]
  obj.includes = bld.env['CPPPATH_USB10']

def clean(ctx):
  os.popen('rm -rf .lock-wscript ./build usbmaestro.node')
