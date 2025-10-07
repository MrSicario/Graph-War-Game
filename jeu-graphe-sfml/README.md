# Jeu Graphe SFML

## Description
Jeu Graphe SFML est un jeu en C++ utilisant la bibliothèque SFML, où deux joueurs s'affrontent sur une plateforme en ligne et en colonne pour gagner en atteignant trois conditions de graphe. Le jeu comprend une interface utilisateur avec un écran d'accueil, un choix de couleur pour chaque joueur, et un système de lancer de dés pour déterminer le premier joueur a commencer.

## Structure du Projet
Le projet est organisé comme suit :

```
jeu-graphe-sfml
├── src
│   ├── main.cpp          # Point d'entrée de l'application
│   ├── jeu.hpp          # Déclaration de la classe Jeu
│   ├── jeu.cpp          # Implémentation de la classe Jeu
│   ├── ui
│   │   ├── accueil.hpp   # Déclaration de la classe Accueil
│   │   ├── accueil.cpp    # Implémentation de la classe Accueil
│   │   ├── choix_couleur.hpp # Déclaration de la classe ChoixCouleur
│   │   ├── choix_couleur.cpp  # Implémentation de la classe ChoixCouleur
│   │   ├── lancer_de.hpp  # Déclaration de la classe LancerDe
│   │   └── lancer_de.cpp  # Implémentation de la classe LancerDe
│   ├── joueur.hpp        # Déclaration de la classe Joueur
│   ├── joueur.cpp        # Implémentation de la classe Joueur
│   ├── graphe.hpp        # Déclaration de la classe Graphe
│   └── graphe.cpp        # Implémentation de la classe Graphe
├── CMakeLists.txt        # Configuration du projet avec CMake
└── README.md             # Documentation du projet
```

## Compilation
Pour compiler le projet, assurez-vous d'avoir CMake et SFML installés sur votre système. Ensuite, exécutez les commandes suivantes dans le terminal :

```
mkdir build
cd build
cmake ..
make
```

## Exécution
Après la compilation, exécutez le jeu avec la commande suivante :

```
./mon_jeu_sfml
```

## Developpeur
Ce projet a été développé par " Mahafale Mahaisoa " 
