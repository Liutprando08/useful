package com.pro.esercizi.demo;

import com.pro.esercizi.geometria.triangolo;

public class demoTriangolo {

  public static void main(String[] args) {
    triangolo tri = new triangolo(1, 1, 1);
    String print = tri.toString();
    String tipo = tri.tipo();
    int per = tri.perimetro();
  }
}
