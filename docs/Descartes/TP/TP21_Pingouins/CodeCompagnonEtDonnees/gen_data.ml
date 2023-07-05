let rec random_genome possible_chars size =
  match size with
  |0 -> []
  |n -> let char = possible_chars.[Random.int ((String.length possible_chars)-1)] in char::(random_genome possible_chars (n-1))
(*Insert finite automata here*)

let sample_poisson lambda =
  let rec poisson_aux p n = (*Tant que p > e^-lambda on incrémente n et on diminue p en le multipliant par un random float dans 0 1*)
    if p > exp (-. lambda) then
      poisson_aux (p *. (Random.float 1.)) (n+1)
    else
      n-1
  in poisson_aux 1. 0
  (* in int_of_float lambda *)

let mutate_char c possible_chars =
  let choice = Random.int 3 in
  match choice with
  |0 ->  [] (*deletion*)
  |1 -> let new_c = possible_chars.[Random.int (String.length possible_chars)] in [new_c;c] (* insertion*)
  |_ -> let new_c = possible_chars.[Random.int (String.length possible_chars)] in [new_c] (*mutation*)

let mutate_genome_once possible_chars genome =
  let n = List.length genome in
  if n > 0 then
    let position = Random.int (List.length genome) in
    let rec find_pos l p =
      match l,p with
      |[],_ -> failwith "erreur"
      |h::t, 0 -> (mutate_char h possible_chars)@t
      |h::t, p -> h::(find_pos t (p-1))
    in find_pos genome position
  else
    [possible_chars.[Random.int (String.length possible_chars)]]

let mutate_genome possible_chars distance genome =
  let number_of_mutation = sample_poisson distance and result = ref genome in
  for i = 0 to number_of_mutation -1 do
    result := mutate_genome_once possible_chars !result
  done;
  !result

let chars = "ATGC"
let g = random_genome chars 10
let g' = mutate_genome chars 1. g
let _ = List.length g

type phyllo_tree = Leaf of string | Node of (float * phyllo_tree) list

let pelican_gris = Leaf "pelican gris"
let pelican_frise = Leaf "pelican frisé"
let pelicans = Node [2., pelican_gris; 3., pelican_frise]
let manchot_du_cap = Leaf  "manchot du cap"
let manchot_pygmee = Leaf "manchot pygmée"
let manchots_1 = Node [2., manchot_du_cap; 2., manchot_pygmee]
let gorfou = Leaf  "gorfou"
let manchot_antipode = Leaf "manchot antipode"
let manchots_2 = Node [2., gorfou; 2., manchot_antipode]
let manchots_3 = Node [2., manchots_1; 2., manchots_2]
let manchot_empereur = Leaf "manchot empereur"
let manchot_royal = Leaf "manchot royal"
let manchots_4 = Node [1., manchot_empereur; 1., manchot_royal]
let manchots_5 = Node [1., manchots_4; 3., manchots_3]
let ardeae = Node [4., pelicans; 7., manchots_5]
let petit_pingouin = Leaf "petit pingouin"
let grand_pingouin = Leaf "grand pingouin"
let pingouins = Node [4., petit_pingouin; 1., grand_pingouin]
let grue_royale = Leaf "grue royale"
let grue_couronnee = Leaf "grue couronnée"
let grues = Node [2., grue_royale; 2., grue_couronnee]
let grues_et_pingouins = Node [8., pingouins; 7., grues]
let oiseaux = Node[15., ardeae; 2., grues_et_pingouins]


let rec add_genomes_to_res (tree: phyllo_tree) (initial_genome: char list) (res: (string * char list) list) (possible_chars:string) :(string * char list) list=
  match tree with
  |Leaf name -> (name, initial_genome)::res
  |Node l -> List.fold_left
               (fun acc (d,t) ->
                  let mutated_genome = mutate_genome possible_chars d initial_genome in
                  (add_genomes_to_res t mutated_genome acc possible_chars))
               res l

let genomes = add_genomes_to_res oiseaux (random_genome chars 100) [] chars

let write_genome g filename =
  let stream = open_out (filename^".gen") in
  let _ = List.iter (fun c -> Printf.fprintf stream "%c" c) g in let _ = Printf.fprintf stream "\n" in close_out stream

let write_all_genomes prefix genomes =
  let header_file = open_out (prefix^"_header.info") in
  let _ = List.iter (fun (name, genome) -> let random_file_name = List.fold_left (fun acc c -> (Printf.sprintf "%c" c)^acc) "" (random_genome "abcdefghijklmnopqrstuvwxyz" 12) in let _ = Printf.fprintf header_file "%s: %s.gen\n" name random_file_name in write_genome genome random_file_name) genomes in close_out header_file 

let _ = write_all_genomes "aves" genomes
