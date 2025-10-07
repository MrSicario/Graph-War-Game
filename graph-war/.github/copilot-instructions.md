## Graph War — guide pour agents IA

But rapide
- Petit jeu JavaFX (local, 2 joueurs) : l'UI est en FXML + contrôleurs JavaFX ; la logique de jeu est dans `graphwar.app.model.GridGame`.

Commandes utiles
- Exigences : Java 17+, Maven 3.8+, JavaFX (version gérée dans `pom.xml`, property `javafx.version = 21.0.3`).
- Exécuter en développement :
  - mvn -f ./pom.xml clean javafx:run
- Construire le jar :
  - mvn -f ./pom.xml package
  - Jar de sortie : `target/graph-war-1.0.0.jar` (attention : exécution standalone peut nécessiter configuration JavaFX/module-path).

Structure & points d'entrée
- Code source Java : `src/app` (le POM définit `sourceDirectory` → `src/app`).
- Ressources (FXML, CSS) : `src/resources` (packagées dans le jar). Fichiers clés :
  - `src/resources/start.fxml` (écran d'accueil)
  - `src/resources/color_select.fxml` (sélection des couleurs)
  - `src/resources/board.fxml` (plateau de jeu)
  - `src/resources/styles.css`
- Entrée de l'app : `graphwar.app.MainApp` (classe Application qui charge `start.fxml`). Il existe aussi `Launcher` avec un main qui lance l'Application.

Architecture & flux principal
- UI (MVC léger) : contrôleurs dans `src/app/ui` :
  - `StartController` → navigue vers la sélection de couleur.
  - `ColorSelectController` → assigne couleurs dans `graphwar.app.AppState` et appelle `Navigation.goTo("/graphwar/board.fxml", node)`.
  - `BoardController` → contient la logique d'affichage (Canvas) et les interactions utilisateur ; il instancie et utilise `GridGame`.
- Modèle : `graphwar.app.model.GridGame` gère les placements, la détection de triangles/rectangles et la condition de victoire.
- État partagé simple : `graphwar.app.AppState` expose deux couleurs statiques (player1/player2) utilisées par les contrôleurs.
- Navigation : `graphwar.util.Navigation.goTo(String fxmlPath, Node anyNode)` charge un FXML et remplace la Scene du Stage courant.

Conventions et pièges spécifiques
- Packaging non standard : le POM met `src/app` comme source et `src/resources` comme resources. Les chemins FXML sont parfois construits avec des chemins absolus (voir `Navigation` et `MainApp`) — vérifiez les chemins de `getResource(...)` si un FXML ne se charge pas.
- Exemples observés :
  - `MainApp` utilise `MainApp.class.getResource("/graph-war/src/resources/start.fxml")`.
  - `Navigation.goTo` concatène `"/graph-war/src/resources/" + fxmlPath`.
  - Les contrôleurs appellent `Navigation.goTo("/graphwar/board.fxml", node)` (notez le paramètre `fxmlPath` attendu : commence par `/graphwar/`).
  -> En pratique, si vous modifiez l'emplacement des FXML, mettez à jour ces chemins.
- FXML ↔ contrôleurs : `fx:controller` dans chaque FXML référence les classes de `graphwar.ui` (ex. `fx:controller="graphwar.ui.BoardController"`). Ne changez pas le package sans mettre à jour le FXML.
- Palette de couleurs : codée en dur dans `ColorSelectController.initialize()` — modifier la palette = éditer ce tableau.
- Taille et grille : `BoardController` fixe `cols = 16`, `rows = 12` ; dessins sur `Canvas` recalculent `cellW/cellH` en fonction des dimensions du canvas.

Debug & workflows de dev
- Si un FXML ne charge pas : lancer `mvn -f ./pom.xml javafx:run` depuis la racine et regarder la stacktrace. Erreurs communes :
  - `NullPointerException` dans initialize() d'un contrôleur → id FXML manquant ou fx:id incorrect.
  - `Unable to load FXML` → chemin erroné passé à `getResource(...)` (vérifier la chaîne exacte utilisée).
- Pour éditer l'UI visuellement : ouvrir les FXML dans Scene Builder (fichiers dans `src/resources`).

Tests & qualité
- Aucune suite de tests fournie. Les modifications de `GridGame` méritent des tests unitaires (comportement déterministe) :
  - scénarios à couvrir : placements hors limites, placement sur cellule occupée, détection de formes (triangle/rectangle), condition de victoire.

Points d'amélioration faciles (pour PRs rapides)
- Uniformiser le chargement des ressources : utiliser toujours `getResource("/start.fxml")` et configurer le packaging pour conserver la même racine, ou centraliser un utilitaire qui résout le chemin correctement.
- Extraire la configuration de grille (cols/rows) en constantes configurables.

Fichiers de référence (exemples cités)
- `src/app/MainApp.java`, `src/app/Launcher.java`
- `src/app/ui/BoardController.java`, `src/app/ui/ColorSelectController.java`, `src/app/ui/StartController.java`
- `src/app/model/GridGame.java`, `src/app/AppState.java` 
- `src/app/util/Navigation.java`

Si quelque chose est ambigu ou si vous voulez que je précise/élargisse une section (ex. rédiger tests unitaires pour GridGame ou corriger les chemins des ressources), dites-le et je fais la PR correspondante.
