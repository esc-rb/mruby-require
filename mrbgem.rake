MRuby::Gem::Specification.new('mruby-require') do |spec|
  spec.licenses = ["MIT"]
  spec.authors = ["Nathan Ladd"]

  spec.summary = "Implements require (incl. $: and $\"), require_relative, and load"
  spec.homepage = "https://github.com/esc-rb/mruby-require"

  spec.linker.libraries << 'dl'

  spec.test_rbfiles = Dir.glob('test/mrbtest/**/*.rb')

  spec.add_dependency 'mruby-compiler'
end
