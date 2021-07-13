assert "File Extension" do
  extension = Controls::Require.file_extension('/path/to/some_file.ext')

  assert_equal('.ext', extension)
end

assert "File Extension, No Extension" do
  extension = Controls::Require.file_extension('/path/to/some_file_without_extension')

  assert_nil(extension)
end

assert "File Extension, No Extension, Windows Directory Separators" do
  extension = Controls::Require.file_extension('\path\to\some_file_without_extension')

  assert_nil(extension)
end

assert "File Extension, No Extension, File Ends With Period" do
  extension = Controls::Require.file_extension('some_file.')

  assert_nil(extension)
end

assert "File Extension, No Extension, Just File" do
  extension = Controls::Require.file_extension('some_file_without_extension')

  assert_nil(extension)
end

assert "File Extension, No Extension, Just a Hidden File" do
  extension = Controls::Require.file_extension('.some_hidden_file')

  assert_nil(extension)
end

assert "File Extension, No Extension, Hidden Directory In Path" do
  extension = Controls::Require.file_extension('/path/.some-hidden-dir/some_file_without_extension')

  assert_nil(extension)
end

assert "File Extension, No Extension, Empty String" do
  extension = Controls::Require.file_extension('/path/to/some_file_without_extension')

  assert_nil(extension)
end
