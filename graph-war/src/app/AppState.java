package app;
import javafx.scene.paint.Color;

public class AppState {
    private static Color player1Color;
    private static Color player2Color;

    public static void setPlayer1Color(Color c) { player1Color = c; }
    public static void setPlayer2Color(Color c) { player2Color = c; }
    public static Color getPlayer1Color() { return player1Color; }
    public static Color getPlayer2Color() { return player2Color; }
}

