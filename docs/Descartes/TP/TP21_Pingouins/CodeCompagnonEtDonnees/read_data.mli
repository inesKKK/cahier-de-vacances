(*Lit les génomes des différentes espèces depuis un fichier header (en .info).
  Renvoie une liste de couple (nom espèce, génome)*)
val read_genome_list_from_header_file : string -> (string * char list) list
