$dirs ||= []
$dirs << __dir__

$other_file ||= 'test/lib/some_dir/some_file.rb'

$raised_error = Controls::Require.load($other_file)

$dirs << __dir__
