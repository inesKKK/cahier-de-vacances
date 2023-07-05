#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

int coord_to_id(int i, int j, int n, int m){ //Pour faire la conversion, il faut les dimensions de la grille.
  return j + m*i;
}
void coord_from_id(int id, int n, int m, int* i, int* j){ //idem ici
  *j = id%m;
  *i = id/m;
}

bool is_adjacent(int id_1, int id_2, int n, int m){//même chose puisqu'on a besoin des coordonnées
  
  int i_1 = 0;
  int j_1 = 0;
  coord_from_id(id_1, n, m, &i_1, &j_1);
  int i_2 = 0;
  int j_2 = 0;
  coord_from_id(id_2, n, m, &i_2, &j_2);
  if (i_1 == i_2){
    if(j_2 == 0){
      return (j_1 == j_2 + 1);
    }
    else if(j_2 == m-1){
      return (j_1 == j_2 - 1);
    }
    else{
      return (j_1 == j_2 + 1) || (j_1 == j_2 - 1);
    }
  }
  else if(j_1 == j_2){
    if(i_2 == 0){
      return (i_1 == i_2 + 1);
    }
    else if (i_2 == n-1){
      return (i_1 == i_2 - 1);
    }
    else{
      return (i_1 == i_2 + 1) || (i_1 == i_2 - 1);
    }
  }
  return false;
}

struct graph_s {int nb_vertices; bool* adj_matrix;}; //adj_matrix est une matrice carrée de taille nb_vertices. Ici la matrice est "linéarisée", c'est à dire représentée par un tableau à une seule dimension. Il est donc nécessaire de calculer l'index d'un couple dans le tableau, mais c'est déjà ce que fait la fonction coord_to_id
typedef struct graph_s graph_t;

graph_t* create_graph(int nb_vertices){
  graph_t* result = (graph_t*)malloc(sizeof(graph_t));
  bool* adj_matrix = (bool*)malloc(sizeof(bool) * nb_vertices * nb_vertices);
  for(int i = 0; i < nb_vertices * nb_vertices; i = i+1){
    adj_matrix[i] = false;
  }
  result->adj_matrix = adj_matrix;
  result->nb_vertices = nb_vertices;
  return result;
}

void free_graph(graph_t* g){ //Il faut libérer chaque ligne et chaque colonne de la matrice d'adjacence.
  free(g->adj_matrix);
  free(g);
}

bool is_neighbor(graph_t* g, int v_1, int v_2){
  return g->adj_matrix[coord_to_id(v_1, v_2, g->nb_vertices, g->nb_vertices)]; //Pour récupérer le coefficient i,j de la matrice, on se sert de la fonction de conversion utilisée pour la grille.
}

void add_edge(graph_t* g, int i, int j){
  g->adj_matrix[coord_to_id(i, j, g->nb_vertices, g->nb_vertices)] = true;
  g->adj_matrix[coord_to_id(j, i, g->nb_vertices, g->nb_vertices)] = true; //la matrice d'adjacence est symétrique
}

void remove_edge(graph_t* g, int i, int j){
  g->adj_matrix[coord_to_id(i, j, g->nb_vertices, g->nb_vertices)] = false;
  g->adj_matrix[coord_to_id(j, i, g->nb_vertices, g->nb_vertices)] = false; //la matrice d'adjacence est symétrique
}

struct labyrinth_s {graph_t* graph; int width; int heigth;};
typedef struct labyrinth_s labyrinth_t;

void remove_wall(labyrinth_t* lab, int i_1, int j_1, int i_2, int j_2){
  int id_1 = coord_to_id(i_1, j_1, lab->width, lab->heigth);
  int id_2 = coord_to_id(i_2, j_2, lab->width, lab->heigth);
  assert(is_adjacent(id_1, id_2, lab->width, lab->heigth));
  add_edge(lab->graph, id_1, id_2);
}

void add_wall(labyrinth_t* lab, int i_1, int j_1, int i_2, int j_2){
  int id_1 = coord_to_id(i_1, j_1, lab->width, lab->heigth);
  int id_2 = coord_to_id(i_2, j_2, lab->width, lab->heigth);
  assert(is_adjacent(id_1, id_2, lab->width, lab->heigth));
  remove_edge(lab->graph, id_1, id_2);
}

labyrinth_t* create_labyrinth(int n, int m, bool is_open){ //quand is_open est vrai, tous les murs du labyrinthe sont rompus
  graph_t* lab_graph = create_graph(n * m);
  labyrinth_t* labyrinth = (labyrinth_t*)malloc(sizeof(labyrinth_t));
  labyrinth->width = n;
  labyrinth->heigth = m;
  labyrinth->graph = lab_graph;
  if(is_open){
    for(int i = 0; i < n; i = i+1){ 
      for(int j = 0; j < m; j = j+1){
        if(i>0){
          remove_wall(labyrinth, i, j, i-1, j);
        }
        if(j>0){
          remove_wall(labyrinth, i, j, i, j-1);
        }
        if(i<n-1){
          remove_wall(labyrinth, i, j, i+1, j);
        }
        if(j<m-1){
          remove_wall(labyrinth, i, j, i, j+1);
        }
      }
    }
  }
  return labyrinth;
}

void free_labyrinth(labyrinth_t* lab){
  free_graph(lab->graph);
  free(lab);
}

void print_labyrinth(labyrinth_t* lab){
  int n = lab->width;
  int m = lab->heigth;
  graph_t* g = lab->graph;
  for(int j = 0; j < 2 * m + 1; j = j+1){//Mur externe du haut
    printf("#");
  }
  printf("\n");
  for(int i = 0; i < n; i = i+1){ //Affichage des lignes
    //Cases du labyrinthe et murs horizontaux (lignes paires sur l'exemple)
    printf("#"); //Mur externe de gauche
    for(int j = 0; j < m; j = j+1){
      printf(" ");//la case elle même
      //Puis la présence ou l'absence de mur vers la droite
      if(j < m-1){//la dernière case ne peut pas avoir de liaison à droite
        if(is_neighbor(g, coord_to_id(i, j, n, m), coord_to_id(i, j+1, n, m))){
          printf(" ");
        }
        else{
          printf("#");
        }
      }
    }
    printf("#\n"); //Mur externe de droite 
    //Laison entre deux lignes (lignes impaires sur l'exemple)
    if(i<n-1){//la dernière ligne n'a pas de liaison avec le la suivante (il n'y a pas de suivante)
      printf("#"); //Mur externe de gauche
      for(int j = 0; j < m; j= j+1){
        if(is_neighbor(g, coord_to_id(i, j, n, m), coord_to_id(i+1, j, n, m))){
          printf(" ");
        }
        else{
          printf("#");
        }
        printf("#");
      }
      printf("\n"); 
    }
  }
  for(int j = 0; j < 2 * m + 1; j= j+1){//Mur externe du bas 
    printf("#");
  }
  printf("\n");
}

struct uf_s {int* partition; int nb_elements;};
typedef struct uf_s uf_t;

uf_t* create_uf(int nb_elements){
  uf_t* uf = (uf_t*)malloc(sizeof(uf_t));
  uf->nb_elements = nb_elements;
  uf->partition = (int*)malloc(sizeof(int) * nb_elements);
  for(int i=0; i< nb_elements; i = i+1){
    uf->partition[i] = i;
  }
  return uf;
}

int find(uf_t* uf, int i){
  assert(i < uf->nb_elements);
  return uf->partition[i];
}

void unite_classes(uf_t* uf, int i, int j){
  assert(i < uf->nb_elements);
  assert(j < uf->nb_elements);
  int i_part = find(uf, i); //Représentant de la classe d'équivalence de i
  int j_part = find(uf, j); // de j
  for(int k = 0; k < uf->nb_elements; k = k+1){
    if(uf->partition[k] == j_part){
      uf->partition[k] = i_part;
    }
  }
}

//Plutôt que de réinventer un algorithme ad-hoc comme celui ci, il aurait été possible d'utiliser un vrai algorithme de mélange, ou bien d'utiliser un tas et d'insérer les arcs dans le tas avec priorité aléatoire puis de les extraire dans l'ordre.
//Ici, on choisit dans l'ordre un élément non ajouté au résultat parmi ceux restants jusqu'à avoir terminé.
void shuffle_array(int* array, int size){
  bool* processed_elements = (bool*)malloc(sizeof(bool)*size);
  for(int i = 0; i<size; i = i+1){
    processed_elements[i] = false;
  }
  int* result =(int*)malloc(sizeof(int)*size);
  for(int i = 0; i < size; i = i+1){
    //Récupération aléatoire d'un élément non déjà utilisé pour le placer en position i
    int random_rank =  1 + (rand() % (size-i)); //numéro aléatoire parmi le nombre d'élément restant à ajouter
    int unprocessed_elements_yet_to_see = random_rank; //Pour choisir le r-ième élément non utilisé on parcourt le tableau original en décrémentant un compteur de 1 quand on voit un élément non choisi et en le laissant identique quand on voit un élément déjà choisi. On peut détecter les éléments choisis via processed_elements.
    int j = 0;
    while(j < size && unprocessed_elements_yet_to_see > 0){
      if(!processed_elements[j]){
        unprocessed_elements_yet_to_see = unprocessed_elements_yet_to_see-1;
      }
      if(unprocessed_elements_yet_to_see > 0){
        j = j+1;
      }
    }
    result[i] = array[j];
    processed_elements[j] = true;
  }
  //On peut maintenant recopier le contenu de result dans array et libérer result
  for(int i = 0; i < size; i = i+1){
    array[i] = result[i];
  }
  free(result);
  free(processed_elements);
}

labyrinth_t* gen_labyrinth(int n, int m){
  labyrinth_t* lab = create_labyrinth(n, m, false);
  int nb_vertices = n*m;
  //Le graphe est symétrique, on ne s'intéresse qu'à la partie i<j. On compte les arcs dans cette partie en les énumérant. Il faut vérifier que les deux sommets de l'arcs sont adjacents en plus de la conditions i<j.
  int nb_edges = 0;
  for(int k = 0; k < nb_vertices * nb_vertices; k = k+1){
    int i = 0;
    int j = 0;
    coord_from_id(k, nb_vertices, nb_vertices, &i, &j); //On récupère i et j à partir du numéro de l'arc
    if(i<j && is_adjacent(i,j, n, m)){
      nb_edges = nb_edges + 1;
    }
  }
  //On recommence le parcours en notant le numéro des arcs conservés dans un tableau
  int* labyrinth_edges = (int*)malloc(sizeof(int)*nb_edges);
  int current_edge = 0;
  for(int k = 0; k < nb_vertices * nb_vertices; k = k+1){
    int i = 0;
    int j = 0;
    coord_from_id(k, nb_vertices, nb_vertices, &i, &j); //On récupère i et j à partir du numéro de l'arc
    if(i<j && is_adjacent(i,j, n, m)){
      labyrinth_edges[current_edge] = k;
      current_edge = current_edge + 1;
    }
  }
  shuffle_array(labyrinth_edges, nb_edges); //la case i contient maintenant l'id du i+1-ième arc à considérer.
  uf_t* uf = create_uf(nb_vertices); //Une structure union find pour garder trace des sommets qui sont dans la même composante connexe
  for(int k = 0; k < nb_edges; k = k+1){//on traite maintenant tous les arcs selon l'algorithme détaillé dans l'enoncé
    int v_1 = 0;
    int v_2 = 0;
    coord_from_id(labyrinth_edges[k], nb_vertices, nb_vertices, &v_1, &v_2); //On récupère les deux sommets aux extremités à partir du numéro de l'arc
    if(!(find(uf, v_1) == find(uf, v_2))){
      unite_classes(uf, v_1, v_2);
      //coordonnées du sommet 1
      int i_1 = 0;
      int j_1 = 0;
      coord_from_id(v_1, n, m, &i_1, &j_1);
      //coordonnées du sommet 2
      int i_2 = 0;
      int j_2 = 0;
      coord_from_id(v_2, n, m, &i_2, &j_2);
      remove_wall(lab, i_1, j_1, i_2, j_2);
    }
  }
  return lab;
}

int main(){
  srand(time(NULL));
  /* srand(0); */
  labyrinth_t* lab1 = create_labyrinth(9, 9, true);
  print_labyrinth(lab1);
  free_labyrinth(lab1);
  labyrinth_t* lab2 = create_labyrinth(9, 9, false);
  remove_wall(lab2, 0, 0, 0, 1);
  remove_wall(lab2, 1, 1, 0, 1);
  print_labyrinth(lab2);
  free_labyrinth(lab2);
  labyrinth_t* lab3 = gen_labyrinth(9,9);
  print_labyrinth(lab3);
  free_labyrinth(lab3);
  return 0;
}
