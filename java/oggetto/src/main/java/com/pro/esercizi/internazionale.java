package com.pro.esercizi;

public class internazionale extends spedizione {
  public internazionale(String codice, double pesoKg) {
    super(codice, pesoKg);
  }

  public double costo() {
    if (zona.equals("EU")) {
      return 5 + 1.2 * pesoKg;
    } else {
      return 12 + 3 * pesoKg + 10;
    }

  }

  public String toString() {
    return "Il costo svldkmds " + costo();
  }

}
