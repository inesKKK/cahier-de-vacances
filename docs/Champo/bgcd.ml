(* Algorithme du PGCD binaire prouvable par Why3
   Inspiré de la version récursive se trouvant à : https://toccata.gitlabpages.inria.fr/toccata/gallery/gcd.en.html
   Pierre Karpman, Automne 2025 (il pleut...)
*)

module BGCD
    use int.Int
    use int.ComputerDivision
    use number.Parity
    use number.Gcd
    use number.Coprime
    use ref.Ref
    
    (* number.Coprime.gcd_coprime dans l'autre sens
       apparemment ça aide...  *)
    lemma gcd_cop
    : forall a b c:int. 0 <= a -> 0 <= b -> 0 <= c
      -> coprime a b -> gcd (a * c) b = gcd c b

    lemma gcd_ee
    : forall a b:int. 0 <= a -> 0 <= b -> even a -> even b
      -> gcd a b = 2 * (gcd (div a 2) (div b 2))

    lemma gcd_2o
    : forall a:int. 0 < a -> odd a
      -> coprime 2 a

    lemma gcd_oe
    : forall a b:int. 0 <= a -> 0 <= b -> odd a
      -> gcd a b = gcd a (2 * b)

    lemma gcd_eo
    : forall a b:int. 0 <= a -> 0 <= b -> odd b
      -> gcd a b = gcd (2 * a) b

    (* plus simple à prouver avec `- 1 * a` au lieu de `- a`... *)
    lemma gcd_sub: forall a b:int. gcd a b = gcd a (b - 1 * a)

    let bgcd (a':int) (b':int) : int
    requires { a' >= 0 /\ b' >= 0 }
    ensures  { result = gcd a' b' }
    =
    let ref r = 1 in
    let ref a = a' in
    let ref b = b' in
    label L0 in
    while (a > 0 && b > 0) do
        variant   { a + b }
        invariant { a >= 0 /\ b >= 0 /\ r > 0 }
        invariant { (r at L0) * gcd (a at L0) (b at L0) = r * (gcd a b) }
        label L1 in
        while (mod a 2 = 0 && mod b 2 = 0) do
            variant   { a + b }
            invariant { a > 0 /\ b > 0 /\ r > 0}
            invariant { a <= a at L1 /\ b <= b at L1  }
            invariant { (r at L1) * gcd (a at L1) (b at L1) = r * (gcd a b) }
            r <- r * 2 ;
            a <- div a 2 ;
            b <- div b 2
        done ;
        while (mod a 2 = 0) do
            variant   { a + b }
            invariant { a > 0 /\ r > 0}
            invariant { a <= a at L1 }
            invariant { even a -> odd b }
            invariant { (r at L1) * gcd (a at L1) (b at L1) = r * (gcd a b) }
            a <- div a 2
        done ;
        while (mod b 2 = 0) do
            variant   { a + b }
            invariant { b > 0 /\ r > 0}
            invariant { b <= b at L1 }
            invariant { even b -> odd a }
            invariant { (r at L1) * gcd (a at L1) (b at L1) = r * (gcd a b) }
            b <- div b 2
        done ;
        if a < b then
            b <- b - a
        else
            let t = b in
            b <- a - b ;
            a <- t
    done ;
    if a > 0 then a * r else b * r
end