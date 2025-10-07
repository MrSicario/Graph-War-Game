package ui;

import app.AppState;
import util.Navigation;
import javafx.fxml.FXML;
import javafx.scene.Node;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.paint.Color;
import java.util.ArrayList;
import java.util.List;

public class ColorSelectController {
    @FXML private Label promptLabel;
    @FXML private Button launchButton;
    @FXML private Button c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12;

    private final List<Button> colorButtons = new ArrayList<>();
    private boolean firstPicked = false;

    @FXML
    public void initialize() {
        colorButtons.addAll(List.of(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12));
        Color[] palette = new Color[] {
            Color.web("#ef4444"), Color.web("#3b82f6"), Color.web("#22c55e"), Color.web("#eab308"),
            Color.web("#a855f7"), Color.web("#f97316"), Color.web("#14b8a6"), Color.web("#f43f5e"),
            Color.web("#06b6d4"), Color.web("#84cc16"), Color.web("#f59e0b"), Color.web("#6366f1")
        };
        for (int i = 0; i < colorButtons.size(); i++) {
            Button b = colorButtons.get(i);
            Color c = palette[i % palette.length];
            b.setUserData(c);
            b.setStyle("-fx-background-color:" + toHex(c) + ";");
        }
        promptLabel.setText("Joueur 1, choisissez votre couleur");
    }

    @FXML
    public void onPickColor(javafx.event.ActionEvent event) {
        Button b = (Button) event.getSource();
        Color chosen = (Color) b.getUserData();
        if (!firstPicked) {
            AppState.setPlayer1Color(chosen);
            firstPicked = true;
            promptLabel.setText("Joueur 2, choisissez votre couleur");
            b.setDisable(true);
        } else {
            AppState.setPlayer2Color(chosen);
            launchButton.setDisable(false);
        }
    }

    @FXML
    public void onLaunch(javafx.event.ActionEvent event) {
        if (AppState.getPlayer1Color() == null || AppState.getPlayer2Color() == null) return;
    Navigation.goTo("/board.fxml", (Node) event.getSource());
    }

    private String toHex(Color c) {
        int r = (int) Math.round(c.getRed() * 255.0);
        int g = (int) Math.round(c.getGreen() * 255.0);
        int b = (int) Math.round(c.getBlue() * 255.0);
        return String.format("#%02x%02x%02x", r, g, b);
    }
}

