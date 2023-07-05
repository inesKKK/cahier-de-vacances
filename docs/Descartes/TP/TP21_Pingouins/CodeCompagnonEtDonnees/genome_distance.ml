let rec edit_distance_memoization x_i y_j table = (*x_i et y_j sont les préfixes de x et y de taille i et j*)
    match (x_i,y_j) with
    |(x_i,y_j) when Hashtbl.mem table (x_i,y_j) -> Hashtbl.find table (x_i,y_j)
    |[], y_j -> let d = List.length y_j in Hashtbl.replace table (x_i, y_j) d; d
    |x_i, [] -> let d = List.length x_i in Hashtbl.replace table (x_i, y_j) d; d
    |c_1::x_i_prefix, c_2::y_j_prefix ->
      let e = if c_1 = c_2 then 0 else 3 in
      let d_1 = edit_distance_memoization x_i_prefix  y_j_prefix table and
      d_2 = (edit_distance_memoization x_i_prefix y_j table) and
      d_3 = (edit_distance_memoization x_i y_j_prefix table) in
      let d =
        if d_1 + e <= 1 + d_2 && d_1 + e <= 1 + d_3 then
          d_1 + e
        else if (1 + d_2 <= d_1 + e && 1 + d_2 <= 1 + d_3) then
          1 + d_2
        else (*(1 + d_3 <= d_1 + e && 1 + d_3 <= 1 + d2*)
          1 + d_3
      in
      (
        Hashtbl.replace table (x_i, y_j) d;
        d
      )

let edit_distance genome_x genome_y =
  let table = Hashtbl.create ((List.length genome_x)*(List.length genome_y)) in
  edit_distance_memoization genome_x genome_y table
