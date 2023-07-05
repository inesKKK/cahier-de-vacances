let fill_id_to_name_array list_genomes =
  if (List.length list_genomes) = 0 then
    [||]
  else
    let result = Array.make (List.length list_genomes) (List.hd list_genomes) in
    let rec fill_array_rec current_genome_list current_position =
      match current_genome_list with
      |[] when current_position = Array.length result -> ()
      |[] -> failwith "list and array have different sizes"
      |_ when current_position = Array.length result -> failwith "list and array have different sizes"
      |(name, genome)::next_genomes -> (result.(current_position) <- (name, genome); fill_array_rec next_genomes (current_position + 1))
    in
    fill_array_rec list_genomes 0;
    result

let make_distance_array genome_array =
  let n = Array.length genome_array in
  let distance_array = Array.make_matrix n n (-1) in
  for i = 0 to (n-1) do
    for j = 0 to (n-1) do
      (* il est inutile de commencer avant i, sinon on va avoir des doublons. On a donc i<j*)
      let n_i,g_i = genome_array.(i) and n_j, g_j = genome_array.(j) in
      distance_array.(i).(j) <- (Genome_distance.edit_distance g_i g_j)
    done
  done;
  distance_array

type cha_tree = Leaf of string * (char list) | Node of cha_tree * cha_tree

let fill_tree_table genome_array =
  let n = Array.length genome_array in
  let tree_table = Hashtbl.create (2*n) in (*C'est la taille attendue pour la table de Hash, cela n'a pas besoin d'être précisément la bonne taille, uniquement le bon ordre de grandeur. On a n arbre initiaux et à chaque fusion on en rajoutera un*)
  for i = 0 to n-1 do
    let n_i, g_i = genome_array.(i) in
    Hashtbl.add tree_table i (Leaf (n_i, g_i))
  done;
  tree_table

let fill_content_table genome_array =
  let n = Array.length genome_array in
  let content_table = Hashtbl.create (2*n) in (*C'est la taille attendue pour la table de Hash, cela n'a pas besoin d'être précisément la bonne taille, uniquement le bon ordre de grandeur. On a n arbre initiaux et à chaque fusion on en rajoutera un*)
  for i = 0 to n-1 do
    Hashtbl.add content_table i [i]
  done;
  content_table

let compute_set_distance_average_link i j distance_array content_table =
  (*Fonction auxiliaire pour calculer la somme des distance de i à chaque génome de list_content*)
  let sum_distance_list i list_content =
    List.fold_left
      (fun acc j ->
         if i <> j then
           acc + distance_array.(i).(j)
         else
           acc
      ) 0 list_content
  in let content_j = (Hashtbl.find content_table j) and content_i = (Hashtbl.find content_table i)
  in float_of_int (List.fold_left (fun acc i -> acc + (sum_distance_list i content_j)) 0 content_i) /. (float_of_int ((List.length content_j) * (List.length content_i)))

let merge_trees i j content_table current_trees_table new_index =
  if i = j then
    failwith "cannot merge a tree with itself"
  else
    let t_i = Hashtbl.find current_trees_table i and t_j = Hashtbl.find current_trees_table j in
    let t_merged = Node (t_i, t_j) and content_i = Hashtbl.find content_table i and content_j = Hashtbl.find content_table j in
    (
      Hashtbl.add content_table new_index (content_i@content_j);
      Hashtbl.remove current_trees_table i;
      Hashtbl.remove current_trees_table j;
      Hashtbl.add current_trees_table new_index t_merged
    )

let get_closest_tree_index distance_array content_table current_trees_table t_index = (*Récupère l'arbre le plus proche de t dans current_trees_table*)
  let best_index = ref None and min_distance = ref None in
  let _ = Hashtbl.iter (fun j t_j ->
      if j <> t_index then
        let d = compute_set_distance_average_link j t_index distance_array content_table in
        match !min_distance, d with
        |None, _ -> (best_index := Some j; min_distance := Some d)
        |Some(min_d), d when d < min_d -> (best_index := Some j; min_distance := Some d)
        |_ -> ()
    ) current_trees_table in
  match !best_index, !min_distance with
  |None, _ -> failwith "no tree found"
  |_, None -> failwith "no tree found"
  |Some(j), Some(d) -> (j,d)

let find_best_merge distance_array current_trees_table content_table =
  let best_i = ref None and best_j = ref None and min_distance = ref None in
  let _ = Hashtbl.iter (fun i t_i ->
      let (j,d) = get_closest_tree_index distance_array  content_table current_trees_table i in
      match !min_distance, d with
      |None, _ -> (best_j:= Some j; best_i:= Some i; min_distance := Some d)
      |Some(min_d), d when d < min_d -> (best_j:= Some j; best_i:= Some i; min_distance := Some d)
      |_ -> ()
    ) current_trees_table in
  match !best_i, !best_j,  !min_distance with
  |None, _, _ -> failwith "no tree found"
  |_, None, _ -> failwith "no tree found"
  |_, _, None -> failwith "no tree found"
  |Some(i), Some(j), Some(d) -> (i,j,d)

let rec build_cha_tree_recursively distance_array content_table current_trees_table n k =
  if n = 1 then
    Hashtbl.find current_trees_table (k-1)
  else
    let (i,j,d) = find_best_merge distance_array current_trees_table content_table in
    let _ = merge_trees i j content_table current_trees_table k in
    build_cha_tree_recursively distance_array content_table current_trees_table (n-1) (k+1)

let build_cha_tree list_genomes =
  let genome_array = fill_id_to_name_array list_genomes in
  let distance_array = make_distance_array genome_array in
  let current_trees_table = fill_tree_table genome_array in
  let content_table = fill_content_table genome_array in
  build_cha_tree_recursively distance_array content_table current_trees_table (Array.length genome_array) (Array.length genome_array)
