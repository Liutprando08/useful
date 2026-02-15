package com.pro.esercizi;

public class express extends spedizione {

  boolean consegnaSabato;

  public express(String codice, double pesoKg) {
    super(codice, pesoKg);
  }

  public double costo() {
    if (sabato = true) {
      return 8 + 2 * pesoKg + 4;

    } else {
      return 5 + 1.2 * pesoKg;

    }
  }

  public String toString() {
    return "il costo sscac " + costo();
  }

}
