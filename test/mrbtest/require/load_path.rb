assert "Require, Load Path, Parse Load Path, Not Set" do
  load_path = Controls::Require.parse_load_paths('')

  assert_equal([], load_path)
end

assert "Require, Load Path, Parse Load Path, Single Path" do
  load_path = Controls::Require.parse_load_paths('/some/absolute/path')

  assert_equal(['/some/absolute/path'], load_path)
end

assert "Require, Load Path, Parse Load Path, Multiple Paths" do
  load_path = Controls::Require.parse_load_paths('/some/absolute/path,some/relative/path,~/home/relative/path')

  control_load_path = [
    '/some/absolute/path',
    'some/relative/path',
    '~/home/relative/path'
  ]

  assert_equal(control_load_path, load_path)
end

assert "Require, Load Path, $: and $LOAD_PATH" do
  assert_kind_of(Array, $LOAD_PATH)
  assert_same($LOAD_PATH, $:)
end
