MRuby::Build.new do |conf|
  conf.toolchain :gcc

  conf.cc do |cc|
    cc.flags = %w(-Werror -Wall -fPIC)
    cc.defines = %w(CONTROLS)
  end

  conf.gem '.'

  conf.gem "test/example-mrbgems/some-feature" do |spec|
    MRuby::DynamicLinking::Mrbgem.(spec)
  end

  conf.gem "test/example-mrbgems/deep-namespace" do |spec|
    MRuby::DynamicLinking::Mrbgem.(spec, 'some_namespace-other_namespace-some_feature.so')
  end

  conf.gem :core => 'mruby-bin-mruby'

  conf.enable_debug
  conf.enable_test
end
