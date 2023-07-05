type op = Insertion | Deletion | Identity | End

let rec diff_memoization x_i y_j distance_table op_table = (*x_i et y_j sont les préfixes de x et y de taille i et j*)
  match (x_i,y_j) with
  |(x_i,y_j) when Hashtbl.mem distance_table (x_i,y_j) -> Hashtbl.find distance_table (x_i,y_j)
  |[], [] -> (* Ici on doit gérer le cas 0,j et i,0 jusqu'à la fin puisqu'on souhaite avoir toutes les opérations.*)
    (
      Hashtbl.replace distance_table (x_i, y_j) 0;
      Hashtbl.replace op_table (x_i, y_j) End;
      0
    )
  |c::x_i_prefix, [] -> (
      let d = diff_memoization x_i_prefix y_j distance_table op_table in
      Hashtbl.replace distance_table (x_i, y_j) d;
      Hashtbl.replace op_table (x_i, y_j) Deletion;
      1+d
    )
  |[], c::y_j_prefix -> (
      let d = diff_memoization  x_i y_j_prefix distance_table op_table in
      Hashtbl.replace distance_table (x_i, y_j) d;
      Hashtbl.replace op_table (x_i, y_j) Insertion;
      1+d
    )
  |c_1::x_i_prefix, c_2::y_j_prefix ->
    let e = if c_1 = c_2 then 0 else 3 in (*Si e vaut 3, cette option ne sera jamais choisie: on peut le faire avec une distance de 2 via une insertion puis une suppression*)
    let d_1 = diff_memoization  x_i_prefix  y_j_prefix distance_table op_table and
      d_2 = (diff_memoization  x_i_prefix y_j distance_table) op_table and
      d_3 = (diff_memoization  x_i y_j_prefix distance_table) op_table in
    let d, op =
      if d_1 + e <= 1 + d_2 && d_1 + e <= 1 + d_3 then
        d_1 + e, Identity
      else if (1 + d_2 <= d_1 + e && 1 + d_2 <= 1 + d_3) then
        1 + d_2, Deletion
      else (*(1 + d_3 <= d_1 + e && 1 + d_3 <= 1 + d2*)
        1 + d_3, Insertion
    in
    (
      Hashtbl.replace distance_table (x_i, y_j) d;
      Hashtbl.replace op_table (x_i, y_j) op;
      d
    )

let diff x_lines y_lines =
  let distance_table = Hashtbl.create ((List.length x_lines)*(List.length y_lines)) in
  let op_table = Hashtbl.create ((List.length x_lines)*(List.length y_lines)) in
  let d = diff_memoization x_lines y_lines distance_table op_table in
  let rec get_op_list op_table current_x current_y =
    (* let _ = List.iter (Printf.printf "%s;") current_x in *)
    (* let _ = Printf.printf "," in *)
    (* let _ = List.iter (Printf.printf "%s;") current_y in *)
    (* let _ = Printf.printf "#\n" in *)
    let current_op = Hashtbl.find op_table (current_x, current_y) in
      match current_op with
      |End -> []
      |Identity -> (Identity, List.hd current_x)::(get_op_list op_table (List.tl current_x) (List.tl current_y))
      |Insertion -> (Insertion, List.hd current_y)::(get_op_list op_table current_x (List.tl current_y))
      |Deletion-> (Deletion, List.hd current_x)::(get_op_list op_table (List.tl current_x) current_y)
  in get_op_list op_table x_lines y_lines, d

let rec print_diff list_op =
  match list_op with
  |[] -> ()
  |(End, _)::_ -> ()
  (* |(Identity, line)::tail -> Printf.printf "=%s\n" line; print_diff tail *)
  |(Identity, line)::tail -> print_diff tail
  |(Insertion, line)::tail -> Printf.printf "+%s\n" line; print_diff tail
  |(Deletion, line)::tail -> Printf.printf "-%s\n" line; print_diff tail

let rec fill_line_list channel_in line_list_ref = (*On ajoute toute ligne lue en tête de line_list_ref tant qu'on est pas à la fin. Une ref est nécessaire puisqu'arrivé à la fin une exception est levée et interrompt l'exécution,*)
  try
    let _ = line_list_ref := (input_line channel_in)::!line_list_ref in fill_line_list channel_in line_list_ref
  with
  |End_of_file -> close_in channel_in

let lines_from_file filename =
  let f = open_in filename and lines = ref [] in let _ = fill_line_list f lines in
  !lines

let main () =
  if Array.length Sys.argv == 3 then
    (
      let filename_1 = Sys.argv.(1) and filename_2 = Sys.argv.(2) in 
      let lines_1 = lines_from_file filename_1 and lines_2 = lines_from_file filename_2 in
      let (op_list,d) = diff lines_1 lines_2 in
      Printf.printf "%d changes\n" d;
      print_diff (List.rev op_list)
    )
  else
    Printf.printf "diff incorrect input: diff <filename_1> <filename_2>\n please retry\n"
let _ = main ()
