let read_genome filename =
  let stream = open_in filename in
  let scan_in_channel = Scanf.Scanning.from_channel stream in
  let rec read_char_in_file scan_in_channel =
    try
      (let c = Scanf.bscanf scan_in_channel "%c" (fun c -> c) in
       if c <> '\n' then
         c::(read_char_in_file scan_in_channel)
      else
        []
      )
    with
    |_ -> []
  in
  let res = read_char_in_file scan_in_channel in
  (close_in stream; res)

let read_header filename =
  let stream = open_in filename in
  let scan_in_channel = Scanf.Scanning.from_channel stream in
  let rec read_filenames scan_in_channel =
    try
      (let (name, genome_filename) = Scanf.bscanf scan_in_channel "%[^:]: %s\n" (fun c1 c2 -> (c1, c2)) in
       (name, genome_filename)::(read_filenames scan_in_channel))
    with
    |_ -> []
  in let res = read_filenames scan_in_channel in
  (close_in stream; res)

let read_genome_list_from_name_list l =
  List.fold_left (fun acc (genome_name, filename) -> let genome = read_genome filename in (genome_name, genome)::acc) [] l

let read_genome_list_from_header_file header_filename = read_genome_list_from_name_list (read_header header_filename)
