package com.pro.esercizi;

import com.pro.esercizi.rettangolo;

public class quadrato extends rettangolo {

  public quadrato(double base, double altezza) {
    super(base, altezza);
  }

  public String toString() {
    return "l'area e: " + area + " il perimetro e: " + perimetro + " la base e: " + base + " l'altezza e:" + altezza;
  }
}
