(*Q2*)
type ('a,'b) hash_table = {data: ('a * 'b) list array; hash_function : 'a -> int}

let init_hash_table n hash_fn = {data = Array.make n []; hash_function = hash_fn}

let rec find_in_list key key_value_list = (* fonction auxiliaire pour parcourir la liste des couples dans une même case *)
  match key_value_list with
  |[] -> None
  |(first_key, first_value)::tail when first_key = key -> Some(first_value)
  |head::tail -> find_in_list key tail

let member key table =
  let index = table.hash_function key in
  match find_in_list key table.data.(index) with
  |None -> false
  |Some(x) -> true

let get_value key table =
  let index = table.hash_function key in
  find_in_list key table.data.(index)

exception Key_in_table

let add_value key value table =
  match get_value key table with
  |None -> let index = table.hash_function key in
    table.data.(index) <- (key,value)::table.data.(index)
  |Some(a) -> raise Key_in_table

let rec remove_from_list key key_value_list = 
  match key_value_list with
  |[] -> []
  |(first_key, first_value)::tail when first_key = key -> tail
  |head::tail -> head::(remove_from_list key tail)

exception Key_not_in_table

let remove_element key table =
  match get_value key table with
  |None -> raise Key_not_in_table
  |Some(a) -> let index = table.hash_function key in
    table.data.(index) <- remove_from_list key table.data.(index)

(*Q6*)
let simple_hash_fn size key = ((key mod size) + size) mod size

let test_table = init_hash_table 5 (simple_hash_fn 5)
let _ = add_value (-5) 15 test_table
let _ = test_table
let _ = add_value 83 17 test_table
let _ = member (-5) test_table
let _ = member 83 test_table
let _ = member 81 test_table
let _ = get_value (-5) test_table
let _ = get_value 83 test_table
let _ = get_value 81 test_table
let _ = remove_element 83 test_table
let _ = get_value 83 test_table

(*Q7*)
let _ = Random.self_init ()

let sample_hash_fn p =
  let a = 1 + (Random.int (p-1)) and b = Random.int (p) in
  let h key = a*key + b mod p in
  h

(*Q8*)
let is_prime n =
  let i = ref 2 in
  while (!i * !i < n && n mod !i <> 0) do
    i := !i+1
  done;
  (!i * !i) > n

let rec pick_larger_prime n = (*Cherche le premier entier plus grand que n*)
  if is_prime(n+1) then
    (n+1)
  else
    pick_larger_prime (n+1)

let rec pick_random_larger_prime n = (*Applique la fonction précédente un nombre aléatoire de fois pour trouver un nombre premier supérieur à n aléatoirement*)
  let r = Random.int 5 and result = ref (pick_larger_prime n) in
  for i = 0 to r do
    result := pick_larger_prime !result
  done;
  !result

let gen_hash_fn size =
  let h_raw = sample_hash_fn (pick_random_larger_prime size) in
  let h_final key = simple_hash_fn size (h_raw key) in
  h_final

(*Q9*)
let f = gen_hash_fn 5
let test_table = init_hash_table 5 (f)
let _ = add_value (-5) 15 test_table
let _ = test_table
let _ = add_value 83 17 test_table
let _ = member (-5) test_table
let _ = member 83 test_table
let _ = member 81 test_table
let _ = get_value (-5) test_table
let _ = get_value 83 test_table
let _ = get_value 81 test_table
let _ = remove_element 83 test_table
let _ = get_value 83 test_table

(*Début Bonus*)
(*Q10*)
let hash_word w p =
  let word_size = String.length w in
  let result = ref 0 in
  let base_power = ref 1 in
  for i = 0 to word_size-1 do 
    result := (!result + ((int_of_char w.[i]) * !base_power mod p)) mod p;
    base_power := (!base_power * 256) mod p;
  done;
  (!result mod p)

let gen_word_hash_fn size =
  let h_raw = gen_hash_fn size in
  let h key = h_raw (hash_word key (pick_random_larger_prime (size * size * 256 * 256))) in
  h

(*Q11*)
let h = gen_word_hash_fn 12

let word_table = init_hash_table 75 h
let _ = add_value "mot_de_passe_1" "p4ssw0rd" word_table
let _ = get_value "mot_de_passe_1" word_table
let _ = remove_element "mot_de_passe_1" word_table
let _ = get_value "mot_de_passe_1" word_table
(*Fin Bonus*)

(*Q13*)
let rec fib_memoisation n hash_table =
  match n with
  |0 -> 0
  |1 -> 1
  |n when member n hash_table -> (
    match get_value n hash_table with
    |None -> failwith "Cas impossible"
    |Some a -> a
  )
  |n -> let fib_n = (fib_memoisation (n-1) hash_table) + (fib_memoisation (n-2) hash_table) in
    let _ = add_value n fib_n hash_table in fib_n

(*Q14*)
let rec fib_naive n =
  match n with
  |0 -> 0
  |1 -> 1
  |n -> fib_naive (n-1) + fib_naive (n-2)


let _ = fib_naive 42
let _ = fib_memoisation 42 (init_hash_table 42 (gen_hash_fn 42))

let print_key_and_value key value =
  Printf.printf "%s %d\n" key value

let table = (Hashtbl.create 5)
let _ = Hashtbl.add table "cinq" 5
let _ = Hashtbl.add table "sept" 7
let _ = Hashtbl.iter (print_key_and_value) table
let _ = table

(*Q15*)
let symmetry table =
  let add_opposite_to_table k v = Hashtbl.replace table (-k) (-v) in
  Hashtbl.iter add_opposite_to_table table

let table = (Hashtbl.create 5)
let _ = Hashtbl.add table 5 5
let _ = Hashtbl.add table 7 7
let _ = symmetry table
let _ = Hashtbl.iter (fun k v -> Printf.printf "%d\n" k) table

(*Partie 2*)

(*Q16*)
let string_to_list word =
  let n = String.length word and result = ref [] in
  for i = 0 to n-1 do
    result := word.[i]::!result;
  done;
  !result

(*Q17*)
let rec edit_distance_memoization x_i y_j table = (*x_i et y_j sont les préfixes de x et y de taille i et j*)
    match (x_i,y_j) with
    |(x_i,y_j) when Hashtbl.mem table (x_i,y_j) -> Hashtbl.find table (x_i,y_j)
    |[], y_j -> let d = List.length y_j in Hashtbl.replace table (x_i, y_j) d; d
    |x_i, [] -> let d = List.length x_i in Hashtbl.replace table (x_i, y_j) d; d
    |c_1::x_i_prefix, c_2::y_j_prefix ->
      let e = if c_1 = c_2 then 0 else 100 in 
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

let edit_distance x y =
  let x_list = string_to_list x and y_list = string_to_list y in
  let table = Hashtbl.create ((List.length x_list)*(List.length y_list)) in
  edit_distance_memoization x_list y_list table

let _ = edit_distance "aaba" "aa"
let _ = edit_distance "bbb" "aaa"

(*Q18*)
type op = Insertion | Deletion | Identity | End

let rec edit_distance_memoization_and_op x_i y_j distance_table op_table = (*x_i et y_j sont les préfixes de x et y de taille i et j*)
  match (x_i,y_j) with
  |(x_i,y_j) when Hashtbl.mem distance_table (x_i,y_j) -> Hashtbl.find distance_table (x_i,y_j)
  |[], [] -> (* Ici on doit gérer le cas 0,j et i,0 jusqu'à la fin puisqu'on souhaite avoir toutes les opérations.*)
    (
      Hashtbl.replace distance_table (x_i, y_j) 0;
      Hashtbl.replace op_table (x_i, y_j) End;
      0
    )
  |c::x_i_prefix, [] -> (
      let d = edit_distance_memoization_and_op x_i_prefix y_j distance_table op_table in
      Hashtbl.replace distance_table (x_i, y_j) d;
      Hashtbl.replace op_table (x_i, y_j) Deletion;
      1+d
    )
  |[], c::y_j_prefix -> (
      let d = edit_distance_memoization_and_op x_i y_j_prefix distance_table op_table in
      Hashtbl.replace distance_table (x_i, y_j) d;
      Hashtbl.replace op_table (x_i, y_j) Insertion;
      1+d
    )
  |c_1::x_i_prefix, c_2::y_j_prefix ->
    let e = if c_1 = c_2 then 0 else 3 in (*Si e vaut 3, cette option ne sera jamais choisie: on peut le faire avec une distance de 2 via une insertion puis une suppression*)
    let d_1 = edit_distance_memoization_and_op x_i_prefix  y_j_prefix distance_table op_table and
      d_2 = (edit_distance_memoization_and_op x_i_prefix y_j distance_table) op_table and
      d_3 = (edit_distance_memoization_and_op x_i y_j_prefix distance_table) op_table in
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

(*Q19*)
let edit_distance_with_op_list x y =
  let x_list = string_to_list x and y_list = string_to_list y in
  let distance_table = Hashtbl.create ((List.length x_list)*(List.length y_list)) in
  let op_table = Hashtbl.create ((List.length x_list)*(List.length y_list)) in
  let d = edit_distance_memoization_and_op x_list y_list distance_table op_table in
  let rec get_op_list op_table current_x current_y =
    let current_op = Hashtbl.find op_table (current_x, current_y) in
      match current_op with
      |End -> []
      |Identity -> (Identity, List.hd current_x)::(get_op_list op_table (List.tl current_x) (List.tl current_y))
      |Insertion -> (Insertion, List.hd current_y)::(get_op_list op_table current_x (List.tl current_y))
      |Deletion-> (Deletion, List.hd current_x)::(get_op_list op_table (List.tl current_x) current_y)
  in get_op_list op_table x_list y_list, d
let x = "aabb" and y = "bbaa"
let op_list,d = edit_distance_with_op_list x y

(*Début Bonus*)
(*Q20*)
let rec print_word_from_list l =
  match l with
  |[] -> ()
  |h::t ->  Printf.printf "%c" h; print_word_from_list t

let rec print_word_from_list_rev l =
  match l with
  |[] -> ()
  |h::t -> print_word_from_list_rev t; Printf.printf "%c" h


let rec print_op_list op_list prefix postfix=
  match op_list with
  |[] -> ()
  |(End, _):: tail -> ()
  |h::tail ->
    (let (new_prefix, new_postfix, header) =
       match h with
       |(Deletion,x) -> List.tl prefix, postfix, (Printf.sprintf "-%c: " x)
       |(Insertion,x) -> prefix, (x::postfix), (Printf.sprintf "+%c: " x)
       |(Identity,x) -> List.tl prefix, (x::postfix), (Printf.sprintf "=%c: " x)
     in
     Printf.printf "%s" header;
     print_word_from_list_rev prefix;
     Printf.printf "|";
     print_word_from_list postfix;
     Printf.printf " -> ";
     print_word_from_list_rev new_prefix;
     Printf.printf "|";
     print_word_from_list new_postfix;
     Printf.printf "\n";
     print_op_list tail (new_prefix) (new_postfix))

let _ = print_op_list op_list (string_to_list x) []
(*Fin Bonus*)

(*Q21*)
let text_to_lines_rev s = (*Parcours du texte jusqu'à trouver un \n. Dès que c'est le cas, on extrait la sous chaine depuis la dernière extraction, on met à jour la position indiquant la dernière extraction et on continue*)
  let result = ref [] and last_line_start = ref 0 in
  for i = 0 to String.length s-1 do
    if s.[i] = '\n' then
      (
        let line_length = (i - !last_line_start) in
        (if line_length > 0 then
           result := (String.sub s !last_line_start (i - !last_line_start))::!result
         else
           result := ""::!result);
        last_line_start := i+1
      );
  done;
  (if !last_line_start < (String.length s -1) then
     result := (String.sub s !last_line_start ((String.length s) -1 - !last_line_start))::!result);
  !result
let _ = text_to_lines_rev "aa\n hfreahfvguora \n u \n\n"
let _ = text_to_lines_rev ""

(*Q22*)
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


let diff x y =
  let x_list = text_to_lines_rev x and y_list = text_to_lines_rev y in
  let distance_table = Hashtbl.create ((List.length x_list)*(List.length y_list)) in
  let op_table = Hashtbl.create ((List.length x_list)*(List.length y_list)) in
  let d = diff_memoization x_list y_list distance_table op_table in
  let rec get_op_list op_table current_x current_y =
    let _ = List.iter (Printf.printf "%s;") current_x in
    let _ = Printf.printf "," in
    let _ = List.iter (Printf.printf "%s;") current_y in
    let _ = Printf.printf "#\n" in
    let current_op = Hashtbl.find op_table (current_x, current_y) in
      match current_op with
      |End -> []
      |Identity -> (Identity, List.hd current_x)::(get_op_list op_table (List.tl current_x) (List.tl current_y))
      |Insertion -> (Insertion, List.hd current_y)::(get_op_list op_table current_x (List.tl current_y))
      |Deletion-> (Deletion, List.hd current_x)::(get_op_list op_table (List.tl current_x) current_y)
  in get_op_list op_table x_list y_list, d

let t1 = "let a = 2 in a+1\nlet b = 2 in b+1" and t2 = "let a = 2 in a+1\nlet b = 3 in b+1"
let _ = text_to_lines_rev t1
let _ = text_to_lines_rev t2
let l, d = diff t1 t2

(*Q23*)
let rec print_diff list_op =
  match list_op with
  |[] -> ()
  |(End, _)::_ -> ()
  |(Identity, line)::tail -> Printf.printf "=%s\n" line; print_diff tail
  |(Insertion, line)::tail -> Printf.printf "+%s\n" line; print_diff tail
  |(Deletion, line)::tail -> Printf.printf "-%s\n" line; print_diff tail

(*Q24*)
let _  = print_diff (List.rev l) (*La liste des opération est dans ordonnée depuis la fin vu la manière dont elle est calculée, il faut renverser celle ci*)

(*Q25*)
let rec fill_line_list channel_in line_list_ref = (*On ajoute toute ligne lue en tête de line_list_ref tant qu'on est pas à la fin. Une ref est nécessaire puisqu'arrivé à la fin une exception est levée et interrompt l'exécution,*)
  let line_read = input_line channel_int in
  let _ = line_list_ref:= line_read::!line_list_ref in
  fill_line_list channel_in line_list_ref

let lines_from_file filename =
  let f = open_in filename and lines = ref []
  try
    fill_line_list f
  with
  |End_of_file -> let _ = close_in f; !lines
(*Q26 dans diff.ml*)
