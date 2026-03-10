package com.pro.esercizi.demo;

import com.pro.esercizi.libro.libro;
import com.pro.esercizi.libro.libroScolastico;

public class demoLibro {

  public static void main(String[] args) {

    libroScolastico scuola = new libroScolastico("dsc", "fdsc", 1111, 12.2, "italano");
    scuola.toString();
    libro lib = new libro("dmkd", "dmkd", 2345, 11.11);
    lib.toString();
  }
}
