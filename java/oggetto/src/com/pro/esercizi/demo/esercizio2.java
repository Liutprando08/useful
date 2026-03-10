package com.pro.esercizi.demo;

import com.pro.esercizi.other.magazzino;

public class esercizio2 {

  public static void main(String[] args) {
    magazzino mag = new magazzino();
    double totale = mag.totaleValore(50, 2);
    mag.stampaScheda("dja", 50, 2);
  }
}
