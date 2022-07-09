(************)
(* Partie 1 *)
(************)

type dict =
  | V
  | N of char * dict * dict


let mots = ["diane"; "dire"; "diva"; "divan"; "divin"; "do"; "dodo";
            "dodu"; "don"; "donc"; "dont"; "ame"; "ames"; "amen"]

let d_mots =
  N ('a',
     N ('m',
        N ('e',
           N ('n',
              N ('$', V, V),
              N ('s',
                 N ('$', V, V),
                 N ('$', V, V))),
           V),
        V),
     N ('d',
        N ('o',
           N ('n',
              N ('t',
                 N ('$', V, V),
                 N ('c',
                    N ('$', V, V),
                    N ('$', V, V))),
              N ('d',
                 N ('u',
                    N ('$', V, V),
                    N ('o',
                       N ('$', V, V),
                       V)),
                 N ('$', V, V))),
           N ('i',
              N ('v',
                 N ('i',
                    N ('n',
                       N ('$', V, V),
                       V),
                    N ('a',
                       N ('n',
                          N ('$', V, V),
                          N ('$', V, V)),
                       V)),
                 N ('r',
                    N ('e',
                       N ('$', V, V),
                       V),
                    N ('a',
                       N ('n',
                          N ('e',
                             N ('$', V, V),
                             V),
                          V),
                       V))),
              V)),
        V))

let exemple =
  ["ame"; "ames"; "amen"; "amer"; "ami"; "amis";
   "amie"; "amies"; "ane"; "anes"; "annee"; "annees";
   "anti"; "avide"; "mais"; "misa"; "misas"; "mesa";
   "same"; "tian"; "tina"; "nain"; "nina"; "isthme";
   "medisant"; "ultime"; "magique"; "essai"; "est"; "qui"]

(* Exercice 1 *)

let rec est_bien_forme d = failwith "à implémenter"


(************)
(* Partie 2 *)
(************)

(* Exercice 3 *)

let mot_of_string s = failwith "à implémenter"

let rec afficher mot = failwith "à implémenter"


(************)
(* Partie 3 *)
(************)

(* Exercice 4 *)

let rec cardinal d = failwith "à implémenter"

let teste_cardinal () =
  assert (cardinal d_mots = 14);
  print_endline "Test ok"

let rec appartient dict mot = failwith "à implémenter"

let appartient_string dict s =
  appartient dict (mot_of_string s)

let teste_appartient () =
  let f s = assert (appartient_string d_mots s) in
  let g s = assert (not (appartient_string d_mots s)) in
  List.iter f mots;
  g "amee";
  g "";
  g "am";
  g "amena";
  g "amen$";
  print_endline "Test ok"

(* Exercice 5 *)

let rec ajouter dict mot = failwith "à implémenter"

let rec dict_of_list u = failwith "à implémenter"

let teste_dict_of_list () =
  let d = dict_of_list mots in
  assert (cardinal d = 14);
  List.iter (fun s -> assert (appartient_string d s)) mots;
  let d_ex = dict_of_list exemple in
  assert (cardinal d_ex = 30);
  List.iter (fun s -> assert (appartient_string d_ex s)) exemple;
  print_endline "Test ok"

(* Exercice 6 *)

let afficher_mots dict = failwith "à implémenter"

let rec longueur_maximale dict = failwith "à implémenter"

let afficher_mots_longs dict n = failwith "à implémenter"


(************)
(* Partie 4 *)
(************)

(* Exercice 7 *)

let lire_fichier f = failwith "à implémenter"


(************)
(* Partie 5 *)
(************)

(* Exercice 8 *)

let calculer_occurrences s = failwith "à implémenter"

let afficher_mots_contenus dict s = failwith "à implémenter"

let afficher_anagrammes dict s = failwith "à implémenter"

(* Exercice 9 *)

let filtrer_mots_contenus dict s = failwith "à implémenter"

let filtrer_mots_contenant dict s = failwith "à implémenter"

let filtrer_anagrammes dict s = failwith "à implémenter"


(************)
(* Partie 6 *)
(************)

(* Exercice 10 *)

let afficher_decompositions dict mot = failwith "à implémenter"

let decompose_anagrammes dict mot = failwith "à implémenter"

(* Exercice 11 *)

let decompose_anagrammes_unique dict mot = failwith "à implémenter"
