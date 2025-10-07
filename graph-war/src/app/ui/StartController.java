package ui;

import util.Navigation;
import javafx.fxml.FXML;
import javafx.scene.Node;

public class StartController {
    @FXML
    public void onStart(javafx.event.ActionEvent event) {
        Navigation.goTo("/color_select.fxml", (Node) event.getSource());
    }
}

