val fill_id_to_name_array : ('a * 'b) list -> ('a * 'b) array
val make_distance_array : ('a * 'b list) array -> int array array
type cha_tree = Leaf of string * char list | Node of cha_tree * cha_tree
val fill_tree_table : (string * char list) array -> (int, cha_tree) Hashtbl.t
val fill_content_table : 'a array -> (int, int list) Hashtbl.t
val compute_set_distance_average_link :
  'a -> 'a -> int array array -> ('a, int list) Hashtbl.t -> float
val merge_trees :
  'a ->
  'a -> ('a, 'b list) Hashtbl.t -> ('a, cha_tree) Hashtbl.t -> 'a -> unit
val get_closest_tree_index :
  int array array ->
  ('a, int list) Hashtbl.t -> ('a, 'b) Hashtbl.t -> 'a -> 'a * float
val find_best_merge :
  int array array ->
  ('a, 'b) Hashtbl.t -> ('a, int list) Hashtbl.t -> 'a * 'a * float
val build_cha_tree_recursively :
  int array array ->
  (int, int list) Hashtbl.t ->
  (int, cha_tree) Hashtbl.t -> int -> int -> cha_tree
val build_cha_tree : (string * char list) list -> cha_tree
