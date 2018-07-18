let extname = path =>
  switch ([%re {|/\.(\w+)$/|}] |> Js.Re.exec(path)) {
  | None => None
  | Some(result) => Js.Nullable.to_opt(Js.Re.captures(result)[0])
  };