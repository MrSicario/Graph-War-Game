package ui;

import app.AppState;
import model.GridGame;
import javafx.fxml.FXML;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;
import java.util.Random;

public class BoardController {
    @FXML private Canvas canvas;
    @FXML private Label diceLabel;
    @FXML private Button diceButton;
    @FXML private Label turnLabel;
    @FXML private Label p1Label, p2Label, p1RectLabel, p2RectLabel, p1TriLabel, p2TriLabel, winnerLabel;

    private final int cols = 16;
    private final int rows = 12;
    private double cellW;
    private double cellH;
    private GridGame game;
    private final Random rng = new Random();
    private boolean starterPicked = false;

    @FXML
    public void initialize() {
        game = new GridGame(cols, rows);
        canvas.setOnMouseClicked(this::handleClick);
        canvas.widthProperty().addListener((o, ov, nv) -> redraw());
        canvas.heightProperty().addListener((o, ov, nv) -> redraw());
        updateLabels();
        redraw();
        if (!starterPicked) {
            onRollDice();
            diceButton.setDisable(true);
        }
    }

    @FXML
    public void onNewGame() {
        game = new GridGame(cols, rows);
        starterPicked = false;
        diceLabel.setText("");
        winnerLabel.setText("");
        redraw();
        updateLabels();
    }

    @FXML
    public void onRollDice() {
        if (starterPicked) return;
        boolean j2Start = rng.nextBoolean();
        if (j2Start) { game.setStartingPlayer(GridGame.Player.J2); diceLabel.setText("J2"); }
        else { game.setStartingPlayer(GridGame.Player.J1); diceLabel.setText("J1"); }
        starterPicked = true;
        updateLabels();
    }

    private void handleClick(MouseEvent e) {
        if (!starterPicked) return;
        int c = (int) Math.floor(e.getX() / cellW);
        int r = (int) Math.floor(e.getY() / cellH);
        if (c < 0 || c >= cols || r < 0 || r >= rows) return;
        boolean ok = game.place(c, r);
        if (ok) {
            redraw();
            updateLabels();
            if (game.hasWon(GridGame.Player.J1)) {
                winnerLabel.setText("J1 a gagné !");
            } else if (game.hasWon(GridGame.Player.J2)) {
                winnerLabel.setText("J2 a gagné !");
            }
        }
    }

    private void redraw() {
        GraphicsContext g = canvas.getGraphicsContext2D();
        double w = canvas.getWidth();
        double h = canvas.getHeight();
        cellW = w / cols;
        cellH = h / rows;
        g.setFill(Color.web("#0f172a"));
        g.fillRect(0, 0, w, h);
        g.setStroke(Color.web("#334155"));
        g.setLineWidth(1.0);
        for (int i = 0; i <= cols; i++) {
            double x = i * cellW;
            g.strokeLine(x, 0, x, h);
        }
        for (int j = 0; j <= rows; j++) {
            double y = j * cellH;
            g.strokeLine(0, y, w, y);
        }
        drawCells(g, GridGame.Player.J1, AppState.getPlayer1Color());
        drawCells(g, GridGame.Player.J2, AppState.getPlayer2Color());
        drawShapes(g, GridGame.Player.J1, AppState.getPlayer1Color());
        drawShapes(g, GridGame.Player.J2, AppState.getPlayer2Color());
    }

    private void drawCells(GraphicsContext g, GridGame.Player p, Color color) {
        if (color == null) color = (p == GridGame.Player.J1 ? Color.RED : Color.BLUE);
        g.setFill(color);
        var cells = game.getCells(p);
        double r = Math.max(4, Math.min(cellW, cellH) * 0.25);
        for (var cell : cells) {
            double cx = cell.col * cellW + cellW / 2.0;
            double cy = cell.row * cellH + cellH / 2.0;
            g.fillOval(cx - r, cy - r, r * 2, r * 2);
        }
    }

    private void updateLabels() {
        Color p1 = AppState.getPlayer1Color();
        Color p2 = AppState.getPlayer2Color();
        p1Label.setTextFill(p1 != null ? p1 : Color.RED);
        p2Label.setTextFill(p2 != null ? p2 : Color.BLUE);
        p1RectLabel.setText(game.getRectangles(GridGame.Player.J1) + "/1");
        p2RectLabel.setText(game.getRectangles(GridGame.Player.J2) + "/1");
        p1TriLabel.setText(game.getTriangles(GridGame.Player.J1) + "/2");
        p2TriLabel.setText(game.getTriangles(GridGame.Player.J2) + "/2");
        String turn = game.getCurrentPlayer() == GridGame.Player.J1 ? "J1" : "J2";
        turnLabel.setText(turn);
    }

    private void drawShapes(GraphicsContext g, GridGame.Player p, Color baseColor) {
        if (baseColor == null) baseColor = (p == GridGame.Player.J1 ? Color.RED : Color.BLUE);
        Color shapeColor = baseColor.deriveColor(0, 1, 1, 0.25);
        g.setFill(shapeColor);
        for (String key : game.getRectangleKeys(p)) {
            String[] parts = key.split(";");
            String[] a = parts[0].split(",");
            String[] b = parts[1].split(",");
            int x1 = Integer.parseInt(a[0]);
            int y1 = Integer.parseInt(a[1]);
            int x2 = Integer.parseInt(b[0]);
            int y2 = Integer.parseInt(b[1]);
            double px = x1 * cellW;
            double py = y1 * cellH;
            double pw = (x2 - x1) * cellW;
            double ph = (y2 - y1) * cellH;
            g.fillRect(px, py, pw, ph);
        }
        g.setStroke(baseColor.deriveColor(0, 1, 1, 0.6));
        g.setLineWidth(2.0);
        for (String key : game.getTriangleKeys(p)) {
            String[] pts = key.split(";");
            double[] xs = new double[3];
            double[] ys = new double[3];
            for (int i = 0; i < 3; i++) {
                String[] t = pts[i].split(",");
                xs[i] = (Integer.parseInt(t[0]) + 0.5) * cellW;
                ys[i] = (Integer.parseInt(t[1]) + 0.5) * cellH;
            }
            g.strokePolygon(xs, ys, 3);
        }
    }
}

