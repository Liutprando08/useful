package com.pro.esercizi.geometria;

public class cerchio extends forna2d {
  @Override
  public double area(double raggio) {
    return raggio * raggio * 3.14;
  }

  @Override
  public String toString() {
    return "cerchio []";
  }

  @Override
  public double perimetro(double raggio) {
    return 2 * raggio * 3.14;
  }
}
