#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

// definition des couleurs
#define RED      "\x1b[31m"
#define GREEN    "\x1b[32m"
#define YELLOW   "\x1b[33m"
#define BLUE     "\x1b[34m"
#define MAGENTA  "\x1b[35m"
#define CYAN     "\x1b[36m"
#define WHITE    "\x1b[0m"
#define SURLIGNE_BLACK      "\x1b[40m"
#define SURLIGNE_RED        "\x1b[41m"
#define SURLIGNE_GREEN      "\x1b[42m"
#define SURLIGNE_YELLOW     "\x1b[43m"
#define SURLIGNE_BLUE       "\x1b[44m"
#define SURLIGNE_MAGENTA    "\x1b[45m"
#define SURLIGNE_CYAN       "\x1b[46m"

// definition des booléens
#define false 0
#define true 1

#define BOARD_ORD 4         // nombre de lignes
#define BOARD_ABS 4         // nombre de colonnes
#define CASE_VIDE 0
#define NOMBRE_ESSAI 170    // permet de choisir le nombre de partie que le bot va simuler a chaque mouvement pour pouvoir gagné
#define VERBOSE 0           // a utiliser pour afficher en detaille les etapes des déplacements avec 2 niveaux

// ############################### A faire #####################################
  //             revoir les niveau de VERBOSE pour l'afichage
  //     regader pour les plateau asymetrique (les bug des mouvements)          <<< seulement si j'ai le tps
  //
// #############################################################################


typedef struct {
  int score;
  int niveau;
  int grille [BOARD_ORD] [BOARD_ABS];
} Plateau;

typedef struct {
  bool haut;
  bool bas;
  bool gauche;
  bool droite;
  bool retour;
} Bloquer;

// Methode permettant d'afficher le Logo 2048 styliser
void Afficher_Logo(){
    puts("");
    puts(MAGENTA   "                          _________    _________    ___    ___    _________                      ");
    puts(BLUE      "                         /______  /   /  ___   /   /  /   /  /   /  ___   /                      ");
    puts(CYAN      "  _____| >                     / /   /  /  /  /   /  /   /  /   /  /  /  /          < |_____     ");
    puts(GREEN     " |          >           ______/ /   /  /  /  /   /  /___/  /   /  /__/  /        <          |    ");
    puts(YELLOW    " |  - - - >    >       / ______/   /  /  /  /   /______   /   /  ___   /      <    < - - -  |    ");
    puts(RED       " |_____     >         / /         /  /  /  /          /  /   /  /  /  /          <     _____|    ");
    puts(BLUE      "       | >           / /_____    /  /__/  /          /  /   /  /__/  /              < |          ");
    puts(MAGENTA   "                    /_______/   /________/          /__/   /________/                            ");
    puts(WHITE "");

}

// Methode permettant d'afficher le plateau mis en parametre et un int pour afficher le plateau gagnant en vert et celui perdant en rouge
void Afficher_Plateau(Plateau plat, int gagner_perdu){  // neutre = 0 // gagner = 1 // perdu = 2 //

  printf(MAGENTA "    Votre Score :%d\n" WHITE, plat.score);
  // ======================= Met la premieres ligne ============================
  if (gagner_perdu == 1) { printf(GREEN "          ╔");
  }else if (gagner_perdu == 2) { printf(RED "          ╔");
  }else { printf("          ╔"); }

  for (int abs = 0; abs < BOARD_ABS -1; abs++) { printf("═════╦"); }
  printf("═════╗");
  puts(WHITE "");
  // ===========================================================================

  for (int ord = 1; ord <= (BOARD_ORD *2) -1 ; ord++) { // repetition pour toute les lignes sauf la premieres et la derniere
    // ============== Met les lignes de separation intermediaire ===============
    if (ord %2 == 0) {

      if (gagner_perdu == 1) { printf(GREEN "          ╠");
      }else if (gagner_perdu == 2) { printf(RED "          ╠");
      }else { printf("          ╠"); }

      for (int abs = 0; abs < BOARD_ABS -1; abs++) { printf("═════╬"); }
      printf("═════╣");
      puts(WHITE "");
    }
    // =========================================================================
    else if (ord %2 == 1) {
      if (gagner_perdu == 1) { printf(GREEN "          ║" WHITE);
      }else if (gagner_perdu == 2) { printf(RED "          ║" WHITE);
      }else { printf("          ║"); }

      for (int abs = 0; abs < BOARD_ABS; abs++) { // repetition pour toutes les cases sauf la derniere
        // ###### Choix de la couleur selon la puissance de 2 à afficher #######
        if (plat.grille [ord /2][abs] == CASE_VIDE) {
          if (gagner_perdu == 1) { printf(GREEN "     ║" WHITE);
          }else if (gagner_perdu == 2) { printf(RED "     ║" WHITE);
          }else { printf("     ║"); }
        }
        else if (plat.grille [ord /2][abs]%13 == 1) {
          printf(WHITE " 2^%d ", plat.grille [ord /2][abs]);
          if (gagner_perdu == 1) { printf(GREEN "║" WHITE);
          }else if (gagner_perdu == 2) { printf(RED "║" WHITE);
          }else { printf(WHITE "║"); }
        }
        else if (plat.grille [ord /2][abs]%13 == 2) {
          printf(YELLOW " 2^%d ", plat.grille [ord /2][abs]);
          if (gagner_perdu == 1) { printf(GREEN "║" WHITE);
          }else if (gagner_perdu == 2) { printf(RED "║" WHITE);
          }else { printf(WHITE "║"); }
        }
        else if (plat.grille [ord /2][abs]%13 == 3) {
          printf(CYAN " 2^%d ", plat.grille [ord /2][abs]);
          if (gagner_perdu == 1) { printf(GREEN "║" WHITE);
          }else if (gagner_perdu == 2) { printf(RED "║" WHITE);
          }else { printf(WHITE "║"); }
        }
        else if (plat.grille [ord /2][abs]%13 == 4) {
          printf(BLUE " 2^%d ", plat.grille [ord /2][abs]);
          if (gagner_perdu == 1) { printf(GREEN "║" WHITE);
          }else if (gagner_perdu == 2) { printf(RED "║" WHITE);
          }else { printf(WHITE "║"); }
        }
        else if (plat.grille [ord /2][abs]%14 == 5) {
          printf(GREEN " 2^%d ", plat.grille [ord /2][abs]);
          if (gagner_perdu == 1) { printf(GREEN "║" WHITE);
          }else if (gagner_perdu == 2) { printf(RED "║" WHITE);
          }else { printf(WHITE "║"); }
        }
        else if (plat.grille [ord /2][abs]%13 == 6) {
          printf(MAGENTA " 2^%d ", plat.grille [ord /2][abs]);
          if (gagner_perdu == 1) { printf(GREEN "║" WHITE);
          }else if (gagner_perdu == 2) { printf(RED "║" WHITE);
          }else { printf(WHITE "║"); }
        }
        else if (plat.grille [ord /2][abs]%13 == 7) {
          printf(RED " 2^%d ", plat.grille [ord /2][abs]);
          if (gagner_perdu == 1) { printf(GREEN "║" WHITE);
          }else if (gagner_perdu == 2) { printf(RED "║" WHITE);
          }else { printf(WHITE "║"); }
        }
        else if (plat.grille [ord /2][abs]%13 == 8) {
          printf(SURLIGNE_YELLOW " 2^%d " SURLIGNE_BLACK, plat.grille [ord /2][abs]);
          if (gagner_perdu == 1) { printf(GREEN "║" WHITE);
          }else if (gagner_perdu == 2) { printf(RED "║" WHITE);
          }else { printf(WHITE "║"); }
        }
        else if (plat.grille [ord /2][abs]%13 == 9) {
          printf(SURLIGNE_CYAN " 2^%d " SURLIGNE_BLACK, plat.grille [ord /2][abs]);
          if (gagner_perdu == 1) { printf(GREEN "║" WHITE);
          }else if (gagner_perdu == 2) { printf(RED "║" WHITE);
          }else { printf(WHITE "║"); }
        }
        else if (plat.grille [ord /2][abs]%13 ==10) {
          printf(SURLIGNE_BLUE " 2^%d" SURLIGNE_BLACK, plat.grille [ord /2][abs]);
          if (gagner_perdu == 1) { printf(GREEN "║" WHITE);
          }else if (gagner_perdu == 2) { printf(RED "║" WHITE);
          }else { printf(WHITE "║"); }
        }
        else if (plat.grille [ord /2][abs]%13 == 11) {
          printf(SURLIGNE_GREEN " 2^%d" SURLIGNE_BLACK, plat.grille [ord /2][abs]);
          if (gagner_perdu == 1) { printf(GREEN "║" WHITE);
          }else if (gagner_perdu == 2) { printf(RED " ║" WHITE);
          }else { printf(WHITE "║"); }
        }
        else if (plat.grille [ord /2][abs]%13 == 12) {
          printf(SURLIGNE_MAGENTA " 2^%d" SURLIGNE_BLACK, plat.grille [ord /2][abs]);
          if (gagner_perdu == 1) { printf(GREEN "║" WHITE);
        }else if (gagner_perdu == 2) { printf(RED "║" WHITE);
          }else { printf(WHITE "║"); }
        }
        else if (plat.grille [ord /2][abs]%13 == 13) {
          printf(SURLIGNE_RED " 2^%d" SURLIGNE_BLACK, plat.grille [ord /2][abs]);
          if (gagner_perdu == 1) { printf(GREEN "║" WHITE);
          }else if (gagner_perdu == 2) { printf(RED "║" WHITE);
          }else { printf(WHITE "║"); }
        }
        else if (plat.grille [ord /2][abs]%13 == 14) {
          printf(SURLIGNE_BLACK " 2^%d" SURLIGNE_BLACK, plat.grille [ord /2][abs]);
          if (gagner_perdu == 1) { printf(GREEN "║" WHITE);
          }else if (gagner_perdu == 2) { printf(RED "║" WHITE);
          }else { printf(WHITE "║"); }
        }
        // #####################################################################
      }
      puts("");
    }// fin if (choix ligne paire ou impaire)
  }// fin for (deuxieme a avant derniere ligne)

  // ======================= Met la derniere ligne =============================
  if (gagner_perdu == 1) { printf(GREEN "          ╚");
  }else if (gagner_perdu == 2) { printf(RED "          ╚");
  }else { printf("          ╚"); }

  for (int abs = 0; abs < BOARD_ABS -1; abs++) { printf("═════╩"); }
  printf("═════╝");
  puts(WHITE "");
  // ===========================================================================

}

// fonction permettant de retourner un nombre random
int my_random(int max, int min){
  return rand()%(max -min) + min;
}
// fonction permettant de faire une pose sur le programme
void my_delay(int i){
    clock_t start,end;
    start = clock();
    while(((end = clock()) -start) <= ((i*CLOCKS_PER_SEC)/1000));
}


// fonction qui test si le plateau mis en paramettre est plein
bool Test_Plateau_Plein(Plateau plat){
  bool grille_pleine = false;
  int compte = 0;
  for (int ord = 0; ord < BOARD_ORD; ord++) {
    for (int abs = 0; abs < BOARD_ABS; abs++) {
      if (plat.grille [ord][abs] != CASE_VIDE) {
        compte += 1;
      }
    }
  }
  if (compte == BOARD_ORD*BOARD_ABS) {  // test pour savoir si le plateau est plein (pour eviter la boucles infini)
    grille_pleine = true;
  }
  if (VERBOSE > 4) {printf("Case totale = %d    Case pleine = %d    Grille pleine : %d\n", BOARD_ORD*BOARD_ABS, compte, grille_pleine);}
  return grille_pleine;
}

// fonction qui test la pressence d'un 2048 ou supperieur sur le plateau !
bool Test_Victoire(Plateau plat){
  bool win = false;
  for (int ord = 0; ord < BOARD_ORD; ord++) {
    for (int abs = 0; abs < BOARD_ABS; abs++) {
      if (plat.grille [ord][abs] >= 11) {
        win = true;
      }
    }
  }
  return win;
}

// fonction qui test si deux plateau son identique
bool Test_Identique(Plateau plat1, Plateau plat2){
  int compteur = 0;
  bool identique;

  for (int ord = 0; ord < BOARD_ORD; ord++) {
    for (int abs = 0; abs < BOARD_ABS; abs++) {
      if (plat1.grille [ord][abs] == plat2.grille [ord][abs]){ compteur += 1;}
    }
  }
  if (compteur == BOARD_ABS*BOARD_ORD){
    identique = true;
  }
  return identique;
}


// fonction permettant d'ajouter une case de manierre random sur la grille
Plateau Ajout_New_Case(Plateau plat){
  bool grille_pleine = Test_Plateau_Plein(plat);
  int c = 0;
  if(grille_pleine == false) {
    int abs = my_random(BOARD_ABS,0);
    int ord = my_random(BOARD_ORD,0);
    while (plat.grille [ord][abs] != CASE_VIDE) {
      if(VERBOSE > 6) {printf("ABS : %d  | ORD : %d\n", abs, ord);}
      abs = my_random(BOARD_ABS,0);
      ord = my_random(BOARD_ORD,0);
      c += 1;
    }
    if(VERBOSE > 4) {printf("definitif ===>  ABS : %d  | ORD : %d  ===> après %d essais\n", abs, ord, c);}
    plat.grille[ord][abs] = plat.niveau + my_random(2,0);
  }
  return plat;
}


//fonctions retournant le plateau mis en paramettre après avoir réalisé le mouvement vers la droite
Plateau Mouvement_Droite(Plateau plat){
  if (VERBOSE > 4) {puts("Mouvement_Droite");}

  for (int ligne = 0; ligne < BOARD_ORD; ligne++) {  // Pour repeter l'operation a chacune des ligne de la grille
  // =================== premier décalage des cases à droite ===================
    for (int i = 0; i < BOARD_ABS -1; i++) {
      for (int Decalage = BOARD_ABS -2; Decalage >= 0; Decalage--) {
        if (plat.grille [ligne][Decalage +1] == CASE_VIDE) {
          plat.grille [ligne][Decalage +1] = plat.grille [ligne][Decalage];
          plat.grille [ligne][Decalage] = CASE_VIDE;
        }
      }
    }
    if (VERBOSE > 5) {
      puts("Premier decalage de toutes les cases à droite:");
      Afficher_Plateau(plat,0);
    }

  // ================== somme les cases ajdacentes identiques ===================
    for (int i = BOARD_ABS -1; i >= 1; i--) {
      if (plat.grille [ligne][i] == plat.grille [ligne][i -1] && plat.grille [ligne][i] != CASE_VIDE) {
        plat.grille [ligne][i] = plat.grille [ligne][i -1] +1;
        plat.grille [ligne][i -1] = CASE_VIDE;
        plat.score += pow(2, plat.grille [ligne][i] -1) *2;
        i -= 1;
      }
    }
    if (VERBOSE > 5) {
      puts("===> Somme :");
      Afficher_Plateau(plat,0);
    }

  // =================== second décalage des cases à droite ===================
    for (int i = 0; i <= (BOARD_ABS/2); i++) {
      for (int Decalage = BOARD_ABS -2; Decalage >= 0; Decalage--) {
        if (plat.grille [ligne][Decalage +1] == CASE_VIDE) {
          plat.grille [ligne][Decalage +1] = plat.grille [ligne][Decalage];
          plat.grille [ligne][Decalage] = CASE_VIDE;
        }
      }
    }
    if (VERBOSE > 5) {
      puts("Second decalage de toutes les cases à droite :");
      Afficher_Plateau(plat,0);
    }
  // ===========================================================================
  }
  return plat;
}
//fonctions retournant le plateau mis en paramettre après avoir réalisé le mouvement vers la gauche
Plateau Mouvement_Gauche(Plateau plat){
  if (VERBOSE > 4) {puts("Mouvement_Gauche");}

  for (int ligne = 0; ligne < BOARD_ORD; ligne++) {  // Pour repeter l'operation a chacune des ligne de la grille
  // =================== premier décalage des cases à gauche ===================
    for (int i = 0; i < BOARD_ABS -1; i++) {  // compteur pour etre sur que tt les casses sont bien decaler au maximum
      for (int Decalage = 1; Decalage <= BOARD_ABS -1; Decalage++) {
        if (plat.grille [ligne][Decalage -1] == CASE_VIDE) {
          plat.grille [ligne][Decalage -1] = plat.grille [ligne][Decalage];
          plat.grille [ligne][Decalage] = CASE_VIDE;
        }
      }
    }
    if (VERBOSE > 5) {
      puts("Premier decalage de toutes les cases à gauche :");
      Afficher_Plateau(plat,0);
    }

  // =================== somme des cases adjacentes égales =====================
    for (int i = 0; i <= BOARD_ABS -2; i++) {
      if (plat.grille [ligne][i] == plat.grille [ligne][i +1] && plat.grille [ligne][i] != CASE_VIDE) {
        plat.grille [ligne][i] = plat.grille [ligne][i +1] +1;
        plat.grille [ligne][i +1] = CASE_VIDE;
        plat.score += pow(2, plat.grille [ligne][i] -1) *2;
        i += 1;
      }
    }
    if (VERBOSE > 5) {
      puts("===> Somme :");
      Afficher_Plateau(plat,0);
    }

  // =================== second décalage des cases à gauche ===================
    for (int i = 0; i <= (BOARD_ABS/2) -1; i++) {
      for (int Decalage = 1; Decalage <= BOARD_ABS -1; Decalage++) {
        if (plat.grille [ligne][Decalage -1] == CASE_VIDE) {
          plat.grille [ligne][Decalage -1] = plat.grille [ligne][Decalage];
          plat.grille [ligne][Decalage] = CASE_VIDE;
        }
      }
    }
    if (VERBOSE > 5) {
      puts("Second decalage de toutes les cases à gauche :");
      Afficher_Plateau(plat,0);
    }
  // ===========================================================================
  }
  return plat;
}
//fonctions retournant le plateau mis en paramettre après avoir réalisé le mouvement vers le haut
Plateau Mouvement_Haut(Plateau plat){
  if (VERBOSE >4) {puts("Mouvement_Haut");}

  for (int colonne = 0; colonne < BOARD_ABS; colonne++) {  // Pour repeter l'operation a chacune des colonne de la grille
  // ================ premier décalage de toute les cases en haut ==============
    for (int i = 0; i < BOARD_ORD -1; i++) {  // compteur pour etre sur que tt les casses sont bien decaler au maximum
      for (int Decalage = 1; Decalage <= BOARD_ORD -1; Decalage++) {
        if (plat.grille [Decalage -1][colonne] == CASE_VIDE) {
          plat.grille [Decalage -1][colonne] = plat.grille [Decalage][colonne];
          plat.grille [Decalage][colonne] = CASE_VIDE;
        }
      }
    }
    if (VERBOSE > 5) {
      puts("Premier decalage de toutes les cases en haut :");
      Afficher_Plateau(plat,0);
    }

  // =================== somme des cases adjacentes égales =====================
    for (int i = 0; i <= BOARD_ORD -2; i++) {
      if (plat.grille [i][colonne] == plat.grille [i +1][colonne] && plat.grille [i][colonne] != CASE_VIDE) {
        plat.grille [i][colonne] = plat.grille [i +1][colonne] +1;
        plat.grille [i +1][colonne] = CASE_VIDE;
        plat.score += pow(2, plat.grille [i][colonne] -1) *2;
        i += 1;
      }
    }
    if (VERBOSE > 5) {
      puts("===> Somme :");
      Afficher_Plateau(plat,0);
    }

  // ================ second décalage de toute les cases en haut ===============
    for (int i = 0; i <= (BOARD_ORD/2) -1; i++) {
      for (int Decalage = 1; Decalage <= BOARD_ORD -1; Decalage++) {
        if (plat.grille [Decalage -1][colonne] == CASE_VIDE) {
          plat.grille [Decalage -1][colonne] = plat.grille [Decalage][colonne];
          plat.grille [Decalage][colonne] = CASE_VIDE;
        }
      }
    }
    if (VERBOSE > 5) {
      puts("Second decalage de toutes les cases en haut :");
      Afficher_Plateau(plat,0);
    }
  // ===========================================================================
  }
  return plat;
}
//fonctions retournant le plateau mis en paramettre après avoir réalisé le mouvement vers le bas
Plateau Mouvement_Bas(Plateau plat){
  if (VERBOSE > 4) {puts("Mouvement_Bas");}

  for (int colonne = 0; colonne < BOARD_ABS; colonne++) {  // Pour repeter l'operation a chacune des colonne de la grille
  // ================ premier décalage de toute les cases en bas ===============
    for (int i = 0; i < BOARD_ORD -1; i++) {  // compteur pour etre sur que tt les casses sont bien decaler au maximum
      for (int Decalage = BOARD_ORD -2; Decalage >= 0; Decalage--) {
        if (plat.grille [Decalage +1][colonne] == CASE_VIDE) {
          plat.grille [Decalage +1][colonne] = plat.grille [Decalage][colonne];
          plat.grille [Decalage][colonne] = CASE_VIDE;
        }
      }
    }
    if (VERBOSE > 5) {
      puts("Premier decalage de toutes les cases en bas :");
      Afficher_Plateau(plat,0);
    }

  // =================== somme des cases adjacentes égales =====================
    for (int i = BOARD_ORD -1; i >= 1; i--) {
      if (plat.grille [i][colonne] == plat.grille [i -1][colonne] && plat.grille [i][colonne] != CASE_VIDE) {
        plat.grille [i][colonne] = plat.grille [i -1][colonne] +1;
        plat.grille [i -1][colonne] = CASE_VIDE;
        plat.score += pow(2, plat.grille [i][colonne] -1) *2;
        i -= 1;
      }
    }
    if (VERBOSE > 5) {
      puts("===> Somme :");
      Afficher_Plateau(plat,0);
    }

  // ================ second décalage de toute les cases en bas ================
    for (int i = 0; i <= (BOARD_ORD/2) -1; i++) {
      for (int Decalage = BOARD_ORD -2; Decalage >= 0; Decalage--) {
        if (plat.grille [Decalage +1][colonne] == CASE_VIDE) {
          plat.grille [Decalage +1][colonne] = plat.grille [Decalage][colonne];
          plat.grille [Decalage][colonne] = CASE_VIDE;
        }
      }
    }
    if (VERBOSE > 5) {
      puts("Second decalage de toutes les cases en bas :");
      Afficher_Plateau(plat,0);
    }
  // ===========================================================================
  }

  return plat;
}


// fonction qui test tout les mouvements pour savoir lesquels sont bloqués
Bloquer Test_Mouvement_Bloquer(Plateau plat){
  Bloquer mouv_bloquer;
  Plateau plat_test;
  int identique = 0;

  plat_test = Mouvement_Haut(plat);
  mouv_bloquer.haut = Test_Identique(plat, plat_test);
  if (VERBOSE > 5) {printf("Mouvement_Haut bloqué !  ==> bloquer = %d\n", mouv_bloquer.haut);}

  plat_test = Mouvement_Bas(plat);
  mouv_bloquer.bas = Test_Identique(plat, plat_test);
  if (VERBOSE > 5) {printf("Mouvement_Bas bloqué !  ==> bloquer = %d\n", mouv_bloquer.bas);}

  plat_test = Mouvement_Gauche(plat);
  mouv_bloquer.gauche = Test_Identique(plat, plat_test);
  if (VERBOSE > 5) {printf("Mouvement_Haut bloqué !  ==> bloquer = %d\n", mouv_bloquer.gauche);}

  plat_test = Mouvement_Droite(plat);
  mouv_bloquer.droite = Test_Identique(plat, plat_test);
  if (VERBOSE > 5) {printf("Mouvement_Haut bloqué !  ==> bloquer = %d\n", mouv_bloquer.droite);}

  return mouv_bloquer;  // stockage dans une scuture spécifique, car impossible de retourné plusieur variables
}


// fonction qui test pour chacun des 4 mouvement possible NOMBRE_ESSAI partie random et fait la moyenne pour savoir lequel est le plus interresant à réaliser (BOT de résoltion du 2048)
Plateau BOT_Resolution(Plateau plat){
  Plateau plat_test, plat_simulation;
  Bloquer bloquer;
  bool plat_plein;
  bool Jeu;
  bool identique = false;
  int mouv;
  int score_haut =0, score_bas =0, score_gauche =0, score_droite =0;
  // bool win;


  for (int i = 0; i < 4; i++) { // permet de faire la simulation pour chacun des quatres mouvements
    for (int j = 0; j < NOMBRE_ESSAI; j++) { // permet de faire NOMBRE_ESSAI de simulation pour chaacun des mouvements

    // ========================= choix du mouvement ============================
      if (i == 0) {        // Mouvement_Haut
        Jeu = true;
        plat_test = Mouvement_Haut(plat);
        identique = Test_Identique(plat, plat_test);
        if (identique == true) {i += 1;}
      }else if (i == 1) {  // Mouvement_Bas
        Jeu = true;
        plat_test = Mouvement_Bas(plat);
        identique = Test_Identique(plat, plat_test);
        if (identique == true) {i += 1;}
      }else if (i == 2) {  // Mouvement_Gauche
        Jeu = true;
        plat_test = Mouvement_Gauche(plat);
        identique = Test_Identique(plat, plat_test);
        if (identique == true) {i += 1;}
      }else if (i == 3) {  // Mouvement_Droite
        Jeu = true;
        plat_test = Mouvement_Droite(plat);
        identique = Test_Identique(plat, plat_test);
        if (identique == true) {i += 1;}
      }

    // ====== simulation d'une partie (jusqu'a la défaite ou la victoire) ======
      plat_simulation = plat_test;
      while (Jeu == true) {
        plat_simulation = Ajout_New_Case(plat_simulation);
        mouv = my_random(4,0);

        if (mouv == 0) {
          //if (TEST == 1) {puts("Random Mouvement_Haut");}
          plat_simulation = Mouvement_Haut(plat_simulation);
        }else if (mouv == 1) {
          //if (TEST == 1) {puts("Random Mouvement_Bas");}
          plat_simulation = Mouvement_Bas(plat_simulation);
        }else if (mouv == 2) {
          //if (TEST == 1) {puts("Random Mouvement_Gauche");}
          plat_simulation = Mouvement_Gauche(plat_simulation);
        }else if (mouv == 3) {
          //if (TEST == 1) {puts("Random Mouvement_Droite");}
          plat_simulation = Mouvement_Droite(plat_simulation);
        }

        plat_plein = Test_Plateau_Plein(plat_simulation);
        if (plat_plein == true) {
          bloquer = Test_Mouvement_Bloquer(plat_simulation);
          if (bloquer.haut == true && bloquer.bas == true && bloquer.gauche == true && bloquer.droite == true) {
            Jeu = false;
            if (VERBOSE == 2) {
              printf("Partie simulation terminer (défaite), score :%d\n", plat_simulation.score);
              Afficher_Plateau(plat_simulation,1);
            }
          }
        }
/*
        win = Test_Victoire(plat_simulation);
        if (win == true) {
          Jeu = false;
          if (VERBOSE == 2) {
            printf("Partie simulation terminer (victoire), score :%d\n", plat_simulation.score);
            Afficher_Plateau(plat_simulation,1);
          }
        }
*/
      // =========== somme des scores finaux de tt les parties simulé ==========
        if (i == 0) {        // Mouvement_Haut
          score_haut += plat_simulation.score;
        }else if (i == 1) {  // Mouvement_Bas
          score_bas += plat_simulation.score;
        }else if (i == 2) {  // Mouvement_Gauche
          score_gauche += plat_simulation.score;
        }else if (i == 3) {  // Mouvement_Droite
          score_droite += plat_simulation.score;
        }
      // =======================================================================
      } // fin While
    } // fin For 0 à NOMBRE_ESSAI
  } // fin for 0 a 3 (pour les 4 mouvements diff)

  if (VERBOSE == 2) {
    printf("score_haut = %d\n", score_haut);
    printf("score_bas = %d\n", score_bas);
    printf("score_gauche = %d\n", score_gauche);
    printf("score_droite = %d\n", score_droite);
  }

// -------- division par NOMBRE_ESSAI pour avoir une moyenne de score ----------
  score_haut = score_haut/NOMBRE_ESSAI;
  score_bas = score_bas/NOMBRE_ESSAI;
  score_gauche = score_gauche/NOMBRE_ESSAI;
  score_droite = score_droite/NOMBRE_ESSAI;

  if (VERBOSE == 2) {
    printf("score_haut = %d\n", score_haut);
    printf("score_bas = %d\n", score_bas);
    printf("score_gauche = %d\n", score_gauche);
    printf("score_droite = %d\n", score_droite);
  }
// -----------------------------------------------------------------------------

// ====== Choix du meilleur mouvement a faire en fonction du score moyen =======
  if (score_haut >= score_bas && score_haut >= score_gauche && score_haut >= score_droite) {
    if (VERBOSE == 1) {puts("mouvement haut");}
    return Mouvement_Haut(plat);
  }else if (score_bas >= score_haut && score_bas >= score_gauche && score_bas >= score_droite) {
    if (VERBOSE == 1) {puts("mouvement bas");}
    return Mouvement_Bas(plat);
  }else if (score_gauche >= score_haut && score_gauche >= score_bas && score_gauche >= score_droite) {
    if (VERBOSE == 1) {puts("mouvement gauche");}
    return Mouvement_Gauche(plat);
  }else if (score_droite >= score_haut && score_droite >= score_bas && score_droite >= score_gauche) {
    if (VERBOSE == 1) {puts("mouvement droite");}
    return Mouvement_Droite(plat);
  }
// =============================================================================
}



int main(int argc, char const *argv[]) {
  srand(time(NULL));     // set le random avant de l'utiliser

  bool Jeux = true;            // Variable qui sert a quitter le jeux
  bool plateau_plein = false;  // variable pour eviter de lancer la
  bool win = false;            // varaible pour savoir si on a gagné
  Bloquer bloquer;             // Variable qui sert a afficher seulement les mouvements disponible
  char action_joueur[1];       // Variable qui définie l'action à réaliser
  char action[1];              // Variable qui sert a faie le choix entre le BOT ou une partie normal


  while (Jeux == true) {
    system("clear");

    Afficher_Logo();
    puts(YELLOW   "   Taper 1 pour lancer le BOT de résolution du 2048");
    puts(YELLOW   "   Taper 2 pour lancer une partie normal");
    puts(YELLOW   "   Taper Q pour quitter le jeu");
    printf(YELLOW "   Choissiser vôtre mode de jeu :");
    scanf("%s" WHITE, action);

    if (action[0] == '1') {  // Choix lancement du BOT

      Plateau Plateau_1;

      // ############## Preparation du plateau avant le lancement ##################
      Plateau_1.niveau = 1;
      Plateau_1.score = 0;
      // Met toutes les cases du plateau en etats vide pour eviter les variables residuelles
      for (int ord = 0; ord < BOARD_ORD; ord++) {
        for (int abs = 0; abs < BOARD_ABS; abs++) {
          Plateau_1.grille [ord][abs] = CASE_VIDE;
        }
      }

      for (int i = 0; i < 2  ; i++) {Plateau_1 = Ajout_New_Case(Plateau_1);} // permet de mettre les N premieres cases au lancement
      // ###########################################################################

      // ===================== Zone de Test BOT_Resolution =========================
      bool Jeu = true;
      bool continu = false;
      while (Jeu == true) {

        my_delay(200); // delai pour plus de lisibiliter pendant la presentation
        system("clear");
        Plateau_1 = BOT_Resolution(Plateau_1);
        Plateau_1 = Ajout_New_Case(Plateau_1);

        Afficher_Plateau(Plateau_1,0 + win);

        win = Test_Victoire(Plateau_1);
        if (win == true && continu == false) {
          system("clear");

          printf(MAGENTA "                Fin de partie\n");
          Afficher_Plateau(Plateau_1, 1);

          puts(YELLOW "Taper C pour continue");
          puts(YELLOW "Taper R pour relancer le BOT");
          puts(YELLOW "Taper Q pour revenir au menu principal");
          puts(YELLOW "Que voulez vous faire :");
          scanf("%c" WHITE, action);

          if (action[0] == 'C' || action[0] == 'c') {
            continu = true;
          }else if (action[0] == 'R' || action[0] == 'r') {
        // ############# Preparation du plateau avant le lancement #############
            Plateau_1.niveau = 1;
            Plateau_1.score = 0;
            // Met toutes les cases du plateau en etats vide pour eviter les variables residuelles
            for (int ord = 0; ord < BOARD_ORD; ord++) {
              for (int abs = 0; abs < BOARD_ABS; abs++) {
                Plateau_1.grille [ord][abs] = CASE_VIDE;
              }
            }

            for (int i = 0; i < 2  ; i++) {Plateau_1 = Ajout_New_Case(Plateau_1);} // permet de mettre les N premieres cases au lancement
        // #####################################################################
          }else if (action[0] == 'Q' || action[0] == 'q') {
            Jeu = false;
          }

        }

        plateau_plein = Test_Plateau_Plein(Plateau_1);
        if (plateau_plein == true){
          bloquer = Test_Mouvement_Bloquer(Plateau_1);
          if(bloquer.haut == true && bloquer.bas == true && bloquer.gauche == true && bloquer.droite == true){
            system("clear");

            printf(MAGENTA "                 Fin de partie\n");
            Afficher_Plateau(Plateau_1, 2 - win);
            printf(YELLOW "Taper R pour relancer le BOT \n");
            printf(YELLOW "Taper Q pour revenir au menu principal \n");
            printf(YELLOW "Que voulez vous faire :");

            scanf("%c" WHITE, action);

            if (action[0] == 'R' || action[0] == 'r') {
              // ############## Preparation du plateau avant le lancement ##################
              Plateau_1.niveau = 1;
              Plateau_1.score = 0;
              // Met toutes les cases du plateau en etats vide pour eviter les variables residuelles
              for (int ord = 0; ord < BOARD_ORD; ord++) {
                for (int abs = 0; abs < BOARD_ABS; abs++) {
                  Plateau_1.grille [ord][abs] = CASE_VIDE;
                }
              }

              for (int i = 0; i < 2  ; i++) {Plateau_1 = Ajout_New_Case(Plateau_1);} // permet de mettre les N premieres cases au lancement
              // ###########################################################################
            }else if (action[0] == 'Q' || action[0] == 'q') {
              Jeu = false;
            }

          }
        }

      }
      // ===========================================================================
    }else if (action[0] == '2') {

      Plateau Plateau_1;            // Plateau actuelle
      Plateau Plateau_2;            // Plateau pour faire un retour en arriere et tester si le joueur et bloquer

      // ############## Preparation du plateau avant le lancement ##################
      Plateau_1.niveau = 1;
      Plateau_1.score = 0;
      // Met toutes les cases du plateau en etats vide pour eviter les variables residuelles
      for (int ord = 0; ord < BOARD_ORD; ord++) {
        for (int abs = 0; abs < BOARD_ABS; abs++) {
          Plateau_1.grille [ord][abs] = CASE_VIDE;
        }
      }

      for (int i = 0; i < 2  ; i++) {Plateau_1 = Ajout_New_Case(Plateau_1);} // permet de mettre les N premieres cases au lancement
      // ###########################################################################


      bool Jeu = true;
      Plateau_2 = Plateau_1;
      while (Jeu == true) {
        system("clear");

        Afficher_Plateau(Plateau_1,0 + win);
        if (bloquer.haut != true) {puts(YELLOW "    Taper H pour faire un mouvement vers le Haut");}
        if (bloquer.bas != true) {puts(YELLOW "    Taper B pour faire un mouvement vers le Bas");}
        if (bloquer.gauche != true) {puts(YELLOW "    Taper G pour faire un mouvement vers le Gauche");}
        if (bloquer.droite != true) {puts(YELLOW "    Taper D pour faire un mouvement vers le Droite");}
        if (bloquer.retour != true) {puts(YELLOW "    Taper R pour faire un retour en arriere (possible une seule fois)");}
        puts(YELLOW "    Taper Q pour quitter la partie");
        printf(YELLOW "    Entrez l'action que vous souhaitez réaliser : ");
        scanf("%s", action_joueur);
        puts("" WHITE);

        if ((action_joueur[0] == 'H' || action_joueur[0] == 'h') && bloquer.haut != true) {
          if (VERBOSE > 3) {puts("Entrer dans 'H' || 'h':");}
          Plateau_2 = Plateau_1;  // recupération de l'ancien plateau pour le retour en arriere
          Plateau_1 = Mouvement_Haut(Plateau_1);
          Plateau_1 = Ajout_New_Case(Plateau_1);
          bloquer.retour = false; // reset le retour en arriere
        }else if ((action_joueur[0] == 'B' || action_joueur[0] == 'b') && bloquer.bas != true) {
          if (VERBOSE > 3) {puts("Entrer dans 'B' || 'b':");}
          Plateau_2 = Plateau_1;  // recupération de l'ancien plateau pour le retour en arriere
          Plateau_1 = Mouvement_Bas(Plateau_1);
          Plateau_1 = Ajout_New_Case(Plateau_1);
          bloquer.retour = false; // reset le retour en arriere
        }else if ((action_joueur[0] == 'G' || action_joueur[0] == 'g') && bloquer.gauche != true) {
          if (VERBOSE > 3) {puts("Entrer dans 'G' || 'g':");}
          Plateau_2 = Plateau_1;  // recupération de l'ancien plateau pour le retour en arriere
          Plateau_1 = Mouvement_Gauche(Plateau_1);
          Plateau_1 = Ajout_New_Case(Plateau_1);
          bloquer.retour = false; // reset le retour en arriere
        }else if ((action_joueur[0] == 'D' || action_joueur[0] == 'd') && bloquer.droite != true) {
          if (VERBOSE > 3) {puts("Entrer dans 'D' || 'd':");}
          Plateau_2 = Plateau_1;  // recupération de l'ancien plateau pour le retour en arriere
          Plateau_1 = Mouvement_Droite(Plateau_1);
          Plateau_1 = Ajout_New_Case(Plateau_1);
          bloquer.retour = false; // reset le retour en arriere
        }else if ((action_joueur[0] == 'R'|| action_joueur[0] == 'r') && bloquer.retour != true) {
          if (VERBOSE > 3) {puts("Entrer dans 'R' || 'r':");}
          Plateau_1 = Plateau_2;
          bloquer.retour = true;
        }else if (action_joueur[0] == 'Q' || action_joueur[0] == 'q') {
          if (VERBOSE > 3) {puts("Entrer dans 'Q' || 'q':");}
          printf(YELLOW "Etes-vous sûr de vouloir quitter la partie Y/N:");
          char quitter[1];
          scanf("%s", quitter);
          if (quitter[0] == 'Y' || quitter[0] == 'y') { Jeu = false; }
        }

        // Test si on a gagné
        win = Test_Victoire(Plateau_1);
        if (win == true) {
          puts(RED "Vous avez gagner BRAVO !!!!!!!");
          puts(YELLOW "Taper C pour continue");
          puts(YELLOW "Taper R pour rejouer");
          puts(YELLOW "Taper Q pour revenir au menu principal");
          printf("Que voulez vous faire :");
          scanf("%c", action);

          if (action[0] == 'R' || action[0] == 'r') {
            // ############## Preparation du plateau avant le lancement ##################
            Plateau_1.niveau = 1;
            Plateau_1.score = 0;
            // Met toutes les cases du plateau en etats vide pour eviter les variables residuelles
            for (int ord = 0; ord < BOARD_ORD; ord++) {
              for (int abs = 0; abs < BOARD_ABS; abs++) {
                Plateau_1.grille [ord][abs] = CASE_VIDE;
              }
            }

            for (int i = 0; i < 2  ; i++) {Plateau_1 = Ajout_New_Case(Plateau_1);} // permet de mettre les N premieres cases au lancement
            // ###########################################################################
            Plateau_2 = Plateau_1;
          }else if (action[0] == 'Q' || action[0] == 'q') {
            Jeu = false;
          }
        }

        plateau_plein = Test_Plateau_Plein(Plateau_1);
        if (plateau_plein == true) {  // Pour éviter qui essais tout les mouvement a chaque deplacement (optimisation)
          bloquer = Test_Mouvement_Bloquer(Plateau_1);

          if(bloquer.haut == true && bloquer.bas == true && bloquer.gauche == true && bloquer.droite == true){
            system("clear");

            printf("Fin de partie\nVous aviez un score de : %d\n", Plateau_1.score);
            Afficher_Plateau(Plateau_1, 2);
            puts(YELLOW "Taper R pour rejouer");
            puts(YELLOW "Taper Q pour revenir au menu principal");
            printf("Que voulez vous faire :");
            scanf("%c", action);

            if (action[0] == 'R' || action[0] == 'r') {
              // ############## Preparation du plateau avant le lancement ##################
              Plateau_1.niveau = 1;
              Plateau_1.score = 0;
              // Met toutes les cases du plateau en etats vide pour eviter les variables residuelles
              for (int ord = 0; ord < BOARD_ORD; ord++) {
                for (int abs = 0; abs < BOARD_ABS; abs++) {
                  Plateau_1.grille [ord][abs] = CASE_VIDE;
                }
              }

              for (int i = 0; i < 2  ; i++) {Plateau_1 = Ajout_New_Case(Plateau_1);} // permet de mettre les N premieres cases au lancement
              // ###########################################################################
              Plateau_2 = Plateau_1;
            }else if (action[0] == 'Q' || action[0] == 'q') {
              Jeu = false;
            }
          }
        }

      } // fin While (Jeux) (pour le joueur)

    }else if(action[0] == 'Q' || action[0] == 'q'){
      printf(YELLOW "Etes-vous sûr de vouloir quitter le jeux Y/N:");
      char quitter[1];
      scanf("%s", quitter);
      if (quitter[0] == 'Y' || quitter[0] == 'y') { Jeux = false; }
    } // fin if (choix du jeu BOT/joueur)
  } // fin du While qui pemet de refaire des partie à l'infini

  return 0;
}
