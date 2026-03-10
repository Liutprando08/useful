package com.pro.esercizi.geometria;

public class quadrato extends rettangolo {

  public quadrato(double base, double altezza) {
    super(base, altezza);
  }

  public String toString() {
    return "l'area e: " + area + " il perimetro e: " + perimetro + " la base e: " + base + " l'altezza e:" + altezza;
  }
}
