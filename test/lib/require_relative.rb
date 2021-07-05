$other_file ||= 'some_dir/some_file.rb'

$required = Controls::Require.require_relative($other_file, __dir__)
