package com.pro.esercizi;

public class express extends spedizione {

  public express(String codice, double pesoKg) {
    super(codice, pesoKg);
  }

  public double costo(boolean consegnaSabato) {
    return 8 + 2 * pesoKg + 4;
  }

}
