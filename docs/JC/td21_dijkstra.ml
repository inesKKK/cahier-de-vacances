(* TP 21 - Dijkstra *)

type sommet = int

type distance = int

type graphe = (sommet * distance) list array

(* Q1 *)


(* Q2 *)


(* Q3 *)
let lireArete s = 
  try 
    Scanf.bscanf (Scanf.Scanning.from_string s) "%d %d %d" (fun x y d-> (x,y,d))
  with _ -> failwith ("ligne invalide : " ^ s)

(* Q4 *)

(* Q5 *)



(* Tas en OCaml *)

(*
type 'a tas = {mutable n : int; mutable maxi : int; mutable data : 'a array; compare : 'a -> 'a -> int}

let nouveau_tas x comp = { n = 0; maxi = 16; data = Array.make 16 x; compare = comp};;

let est_vide tas = tas.n = 0;;

let echange tab i j = 
  let tmp = tab.(j) in
  tab.(j) <- tab.(i);
  tab.(i) <- tmp;;

let rec descendre tas i =
  if (2 * i + 1 < tas.n) then
    let f = ref (2 * i + 1) in (* indice du fils gauche *)
    if (2 * i + 2 < tas.n) && tas.compare tas.data.(2*i+2) tas.data.(2*i+1) < 0 then
      f := 2 * i + 2; (* indice du fils droit *)
    if tas.compare tas.data.(!f) tas.data.(i) < 0 then begin
      echange tas.data !f i;
      descendre tas !f;
    end;
;;

let retirer_min tas = 
  if tas.n = 0 then failwith "Tas vide" else
    let x = tas.data.(0) in
    tas.n <- tas.n - 1;
    tas.data.(0) <- tas.data.(tas.n);
    (* tamiser vers le bas *)
    descendre tas 0;
    x
  ;;


let rec remonter tas i =
  if i > 0 then
    let p = (i-1) / 2 in (* indice du pere *)
    if tas.compare tas.data.(i) tas.data.(p) < 0 then begin
      echange tas.data i p;
      remonter tas p;
    end
;;

let ajouter tas x = 
   if tas.n = tas.maxi then begin
      tas.maxi <- 2*tas.maxi;
      let data2 = Array.init tas.maxi (fun i -> if i < tas.n then tas.data.(i) else x) in
      tas.data <- data2;
   end;
   tas.data.(tas.n) <- x;
   tas.n <- tas.n + 1;
   (* tamiser vers le haut *)
   remonter tas (tas.n - 1)
;;

let comp (d1, s1) (d2, s2) = d1 - d2;;

let creer_tas () = nouveau_tas (0,0) comp;;
*)

(* Q6 *)


(* Algo Dijkstra *)

(* Q7 *)

(* Q8 *)

(* Q9 *)

