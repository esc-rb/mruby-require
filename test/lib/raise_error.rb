$dirs ||= []
$dirs << __dir__

if $error != false
  $error ||= RuntimeError.new('Some Error')

  raise $error
end
