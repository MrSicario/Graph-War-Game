# Graph War (JavaFX)

Mini‑jeu JavaFX sans base de données. Deux joueurs (Rouge et Bleu) revendiquent les nœuds d’un graphe d'une grille  ; la formation de triangles (1) et de rectangles (2) rapporte des points et peut mener à la victoire. . Une arête entre deux nœuds de même couleur vaut 1 point.


## Fonctionnalités
- Jeu local pour 2 joueurs (J1 / J2).
- Sélection de couleurs pour les deux joueurs via l'écran de configuration.
- Plateau rendu sur un Canvas avec une grille (16x12 ).
- Placement alterné de pions (cercles) sur les cellules.
- Détection automatique de triangles droits et rectangles formés par les pions d'un même joueur.
- Dashboard affichant le nombre de triangles / rectangles et le joueur courant.
- Lancer aléatoire du joueur qui commence (bouton "Lancer le dé").

## Prérequis
- Java 17+ (JDK).
- Maven 3.8+
- (Optionnel) SDK JavaFX si vous voulez lancer le jar manuellement.

## Commandes utiles
- Compiler :
```
mvn -f ./pom.xml -DskipTests compile
```

- Lancer en mode développement (recommandé) :
```
mvn -f ./pom.xml clean javafx:run
```

- Packager :
```
mvn -f ./pom.xml package
```

## Structure du projet
Le projet utilise un layout non standard (les sources Java sont dans `src/app` et les ressources FXML/CSS dans `src/resources`). Le POM a été adapté pour cette organisation (voir `<sourceDirectory>` et `<resources>`).

Arborescence principale (fichiers clés) :
```
graph-war/
	pom.xml							#configuration Maven (JDK 17, dépendances JavaFX, plugin `javafx-maven-plugin`, sourceDirectory = `src/app`).
	src/
		app/
			MainApp.java            # Classe Application (point d'entrée)
			Launcher.java           # Petit launcher (main)
			AppState.java           # État global (couleurs des joueurs)
			ui/
				StartController.java  # Contrôleur pour l'écran d'accueil
				ColorSelectController.java # Contrôleur pour sélection des couleurs
				BoardController.java  # Contrôleur du plateau (Canvas + logique d'UI)
			model/
				GridGame.java         # Logique du jeu : placements et détection de formes
			util/
				Navigation.java       # Utilitaire pour changer d'écran (chargement FXML)
		resources/
			start.fxml              # FXML écran d'accueil
			color_select.fxml       # FXML sélection de couleurs
			board.fxml              # FXML plateau de jeu
			styles.css              # Styles
```

## Developpeur
Ce projet a été développé par " Mahafale Mahaisoa " 

