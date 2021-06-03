begin
  Controls
rescue NameError
  raise NotImplementedError, "Controls must be included in compilation via `-D CONTROLS=on'"
end
