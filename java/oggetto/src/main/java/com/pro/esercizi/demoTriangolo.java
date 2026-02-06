package com.pro.esercizi;

public class demoTriangolo {

  public static void main(String[] args) {
    triangolo tri = new triangolo(1, 1, 1);
    String print = tri.toString();
    String tipo = tri.tipo();
    int per = tri.perimetro();
  }
}
