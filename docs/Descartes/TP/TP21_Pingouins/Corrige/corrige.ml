#load "read_data.cmo";;
#load "genome_distance.cmo";;
#load "cha.cmo";;

let genome_list = Read_data.read_genome_list_from_header_file "aves_header.info"
let (n1,g1) = List.hd (genome_list)
let (n2,g2) = List.hd (List.tl genome_list)
let d = (Genome_distance.edit_distance g1 g2)

let t = Cha.build_cha_tree (Read_data.read_genome_list_from_header_file "aves_header.info")

let rec print_tree t =
  match t with
  |Cha.Leaf (s, _) -> Printf.printf "%s" s
  |Cha.Node (t1, t2) -> (Printf.printf "("; print_tree t1; Printf.printf " ; "; print_tree t2; Printf.printf ")")

let _ = print_tree t
