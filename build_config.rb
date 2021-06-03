MRuby::Build.new do |conf|
  conf.toolchain :gcc

  conf.cc do |cc|
    cc.flags = %w(-Werror -Wall -fPIC)
    cc.defines = %w(CONTROLS)
  end

  conf.gem '.'

  conf.gem :core => 'mruby-bin-mruby'

  conf.enable_debug
  conf.enable_test
end
