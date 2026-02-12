package com.pro.esercizi;

public class rettangolo {
  protected double base;
  protected double altezza;
  protected double area;
  protected double perimetro;

  public rettangolo(double base, double altezza) {
    this.base = base;
    this.altezza = altezza;
  }

  public void setBase(Double base) {
    this.base = base;
  }

  public void setAltezza(Double altezza) {
    this.altezza = altezza;
  }

  public double calcolaArea(double base, double altezza) {

    double area = base * altezza;
    this.area = area;
    return area;

  }

  public double calcolaPerimetro(double base, double altezza) {
    double perimetro = 2 * (base + altezza);
    this.perimetro = perimetro;
    return perimetro;
  }

  public boolean isQuadrato(double altezza, double base) {
    if (altezza == base) {
      return true;
    } else {
      return false;
    }
  }

  public boolean sonoUguali(double altezza1, double base1, double altezza2, double base2) {
    if (altezza1 == altezza2 && base1 == base2) {
      return true;
    } else {
      return false;
    }
  }

  public String toString() {

    return "l'area e: " + area + " il perimetro e: " + perimetro + " la base e: " + base + " l'altezza e:" + altezza;

  }
}
