package util;

import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;

public final class Navigation {
    private Navigation() {}

    public static void goTo(String fxmlPath, Node anyNode) {
        try {
            String resource = normalizePath(fxmlPath);
            FXMLLoader loader = new FXMLLoader(Navigation.class.getResource(resource));
            Parent root = loader.load();
            Stage stage = (Stage) anyNode.getScene().getWindow();
            stage.setScene(new Scene(root));
            stage.centerOnScreen();
        } catch (IOException e) {
            throw new RuntimeException("Unable to load FXML: " + fxmlPath, e);
        }
    }

    private static String normalizePath(String fxmlPath) {
        if (fxmlPath == null) return null;
        String p = fxmlPath;
        // If someone passed an absolute path inside the repo, extract the filename after src/resources/
        int idx = p.indexOf("src/resources/");
        if (idx != -1) {
            p = p.substring(idx + "src/resources/".length());
        }
        // Remove common prefixes used in this project
        if (p.startsWith("/graphwar/")) p = p.substring("/graphwar/".length());
        if (p.startsWith("graphwar/")) p = p.substring("graphwar/".length());
        if (p.startsWith("/graph-war/")) p = p.substring("/graph-war/".length());
        if (p.startsWith("graph-war/")) p = p.substring("graph-war/".length());
        // Ensure leading slash for Class.getResource
        if (!p.startsWith("/")) p = "/" + p;
        return p;
    }
}

