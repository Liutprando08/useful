package com.pro.esercizi;

abstract class forna2d {

  abstract double area(double raggio);

  abstract double perimetro(double raggio);

  @Override
  public String toString() {
    return "forna2d []";
  }

}

class cerchio extends forna2d {
  @Override
  double area(double raggio) {
    return raggio * raggio * 3.14;
  }

  @Override
  public String toString() {
    return "cerchio []";
  }

  @Override
  double perimetro(double raggio) {
    return 2 * raggio * 3.14;
  }
}
