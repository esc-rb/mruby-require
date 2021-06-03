ENV['MRUBY_BUILD_DIR'] ||= File.expand_path('build', __dir__)
ENV['MRUBY_CONFIG'] ||= File.expand_path('build_config.rb', __dir__)
ENV['INSTALL_DIR'] ||= File.expand_path('bin', __dir__)

ENV.fetch('MRUBY_ROOT') do
  puts "MRUBY_ROOT must be set... exiting"
  exit(false)
end

module MRuby
  module DynamicLinking
    module Mrbgem
      def self.call(spec, dynamic_library_file=nil, target: nil)
        target ||= ::MRuby.targets.fetch('host')

        unless target.is_a?(TargetExtension)
          target.extend(TargetExtension)
        end

        target.dynamically_linked_mrbgem!(spec, dynamic_library_file)
      end

      module TargetExtension
        def dynamically_linked_mrbgem!(spec, dynamic_library_file)
          remove_statically_linked_mrbgem(spec)

          add_dynamically_linked_mrbgem(spec, dynamic_library_file)
        end

        def remove_statically_linked_mrbgem(spec)
          remove_mrbgem(spec.name)

          libmruby_objs.delete_if do |objs|
            objs.equal?(spec.objs)
          end

          if presym_enabled?
            spec.compilers.each do |cc|
              cc.include_paths << "#{build_dir}/include"
            end
          end

          spec.setup_compilers
        end

        def remove_mrbgem(mrbgem_name)
          gems.instance_exec(self) do |build|
            new_ary = @ary.reject do |gem|
              gem.name == mrbgem_name
            end

            @ary = new_ary
          end
        end

        def add_dynamically_linked_mrbgem(spec, dynamic_library_file=nil)
          dynamic_library_file ||= "#{spec.name}.so"

          library_path = "#{build_dir}/lib/#{dynamic_library_file}"

          file library_path => libfile("#{build_dir}/lib/libmruby_core")

          file library_path => spec.objs do
            libraries = ['mruby_core']

            library_paths = ["#{build_dir}/lib"]

            flags = ['-shared']

            linker.run(library_path, spec.objs, libraries, library_paths, flags)
          end

          products << library_path

          mrbgem = Mrbgem.new(spec, dynamic_library_file)
          dynamically_linked_mrbgems << mrbgem
          mrbgem
        end

        def print_build_summary
          display_names = dynamically_linked_mrbgems.map do |mrbgem|
            display_spec = mrbgem.spec.dup

            display_name = "#{mrbgem.spec.name} (#{mrbgem.dynamic_library_file})"
            display_spec.name = display_name

            gems << display_spec

            display_name
          end

          super()

        ensure
          display_names.each do |display_name|
            remove_mrbgem(display_name)
          end
        end

        def dynamically_linked_mrbgems
          @dynamically_linked_mrbgems ||= []
        end

        Mrbgem = Struct.new(:spec, :dynamic_library_file)
      end
    end
  end
end
