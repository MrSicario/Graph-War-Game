package model;

import java.util.HashSet;
import java.util.Objects;
import java.util.Set;

public class GridGame {
    public enum Player { J1, J2 }

    public static final class Cell {
        public final int col;
        public final int row;
        public Cell(int col, int row) { this.col = col; this.row = row; }
        @Override public boolean equals(Object o) { if (this == o) return true; if (!(o instanceof Cell)) return false; Cell c = (Cell) o; return col == c.col && row == c.row; }
        @Override public int hashCode() { return Objects.hash(col, row); }
    }

    private final int cols;
    private final int rows;
    private Player currentPlayer = Player.J1;
    private int movesPlaced = 0;
    private final Set<Cell> j1Cells = new HashSet<>();
    private final Set<Cell> j2Cells = new HashSet<>();

    private final Set<String> j1Triangles = new HashSet<>();
    private final Set<String> j2Triangles = new HashSet<>();
    private final Set<String> j1Rectangles = new HashSet<>();
    private final Set<String> j2Rectangles = new HashSet<>();

    public GridGame(int cols, int rows) {
        this.cols = cols; this.rows = rows;
    }

    public int getCols() { return cols; }
    public int getRows() { return rows; }
    public Player getCurrentPlayer() { return currentPlayer; }

    public boolean place(int col, int row) {
        if (!inBounds(col, row)) return false;
        Cell c = new Cell(col, row);
        if (j1Cells.contains(c) || j2Cells.contains(c)) return false;
        if (currentPlayer == Player.J1) j1Cells.add(c); else j2Cells.add(c);
        detectNewShapes(currentPlayer, c);
        toggle();
        movesPlaced++;
        return true;
    }

    private boolean inBounds(int c, int r) { return c >= 0 && c < cols && r >= 0 && r < rows; }

    private void toggle() { currentPlayer = (currentPlayer == Player.J1) ? Player.J2 : Player.J1; }

    private Set<Cell> setOf(Player p) { return p == Player.J1 ? j1Cells : j2Cells; }
    private Set<String> triSet(Player p) { return p == Player.J1 ? j1Triangles : j2Triangles; }
    private Set<String> rectSet(Player p) { return p == Player.J1 ? j1Rectangles : j2Rectangles; }

    private void detectNewShapes(Player p, Cell pivot) {
        Set<Cell> S = setOf(p);
        for (Cell vertical : S) {
            if (vertical.col == pivot.col && vertical.row != pivot.row) {
                for (Cell horizontal : S) {
                    if (horizontal.row == pivot.row && horizontal.col != pivot.col) {
                        Cell diag = new Cell(horizontal.col, vertical.row);
                        if (S.contains(diag)) {
                            String rectKey = rectKey(pivot, vertical, horizontal, diag);
                            rectSet(p).add(rectKey);
                        }
                        String triKey = triKey(pivot, vertical, horizontal);
                        triSet(p).add(triKey);
                    }
                }
            }
        }
    }

    private String triKey(Cell a, Cell b, Cell c) {
        int[] xs = new int[] { a.col, b.col, c.col };
        int[] ys = new int[] { a.row, b.row, c.row };
        for (int i = 0; i < 3; i++) {
            for (int j = i + 1; j < 3; j++) {
                if (xs[j] < xs[i] || (xs[j] == xs[i] && ys[j] < ys[i])) {
                    int tx = xs[i], ty = ys[i]; xs[i] = xs[j]; ys[i] = ys[j]; xs[j] = tx; ys[j] = ty;
                }
            }
        }
        return xs[0]+","+ys[0]+";"+xs[1]+","+ys[1]+";"+xs[2]+","+ys[2];
    }

    private String rectKey(Cell p, Cell v, Cell h, Cell d) {
        int x1 = Math.min(p.col, d.col);
        int x2 = Math.max(p.col, d.col);
        int y1 = Math.min(p.row, d.row);
        int y2 = Math.max(p.row, d.row);
        return x1+","+y1+";"+x2+","+y2;
    }

    public int getTriangles(Player p) { return triSet(p).size(); }
    public int getRectangles(Player p) { return rectSet(p).size(); }

    public boolean hasWon(Player p) { return getRectangles(p) >= 1 && getTriangles(p) >= 2; }

    public Set<Cell> getCells(Player p) { return p == Player.J1 ? j1Cells : j2Cells; }

    public void setStartingPlayer(Player p) {
        if (movesPlaced != 0) return;
        currentPlayer = p;
    }

    public Set<String> getRectangleKeys(Player p) { return Set.copyOf(rectSet(p)); }
    public Set<String> getTriangleKeys(Player p) { return Set.copyOf(triSet(p)); }
}

