package com.pro.esercizi;

public class triangolo {
  private int lat1;
  private int lat2;
  private int lat3;

  public triangolo(int lat1, int lat2, int lat3) {
    this.lat1 = lat1;
    this.lat2 = lat2;
    this.lat3 = lat3;

  }

  public int perimetro() {

    int perimetro = lat1 + lat2 + lat3;
    return perimetro;
  }

  public String tipo() {
    String tipo;
    if (lat1 == lat2 || lat1 == lat3 || lat2 == lat3) {
      tipo = "isoscele";
    } else if (lat2 == lat1 && lat2 == lat3) {
      tipo = "equilatero";
    } else {
      tipo = "scaleno";
    }
    return tipo;
  }

  public String toString() {
    String stampa = ("perimetro = " + lat1 + lat2 + lat3 + " Area = " + lat1 * lat2 / 2);
    return stampa;
  }
}
