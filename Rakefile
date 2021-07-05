load 'mruby.rake'

load File.join(ENV.fetch('MRUBY_ROOT'), 'Rakefile')

task :copy_dynamic_libraries => :all do
  [
    'build/host/lib/some_feature.so',
    'build/host/lib/some_namespace-other_namespace-some_feature.so'
  ].each do |source|
    target = source.pathmap('test/lib/%f')

    sh "cp -v #{source} #{target}"
  end

  # Ideally, not_an_mrbgem.so would be a dynamic library that
  # doesn't contain an MRuby gem (`mrbgem') at all. However,
  # MRuby's build system cannot produce a library file withou
  # an mrbgem.
  #
  # 'some_feature.so' contains an MRuby gem (`mrbgem') called
  # 'some_feature'. The symmetry between the file name and
  # the gem name is necessary to detect that the .so file is
  # in fact a mrbgem.
  #
  # not_an_mrbgem.so will contain an mrbgem called 'some_feature',
  # and the name mismatch will prevent it from being detected as
  # an mrbgem.
  sh "cp -v test/lib/some_feature.so test/lib/not_an_mrbgem.so"

  touch "test/lib/not_a_dynamic_library.so"
end

task :test => :copy_dynamic_libraries

task :remove_dynamic_libraries do
  sh 'rm -f test/lib/*.so'
end

file 'bin/mrbc' => :all

file 'test/lib/some_feature.mrb' => ['test/lib/some_feature.rb', 'bin/mrbc'] do |task|
  sh "bin/mrbc -o #{task.name} #{task.prerequisites[0]}"
end
task :test => ['test/lib/some_feature.mrb']

task :remove_mrb_files do
  sh 'rm -f test/lib/*.mrb'
end
task :clean => [:remove_dynamic_libraries, :remove_mrb_files]
