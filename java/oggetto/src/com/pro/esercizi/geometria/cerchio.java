package com.pro.esercizi.geometria;

import com.pro.esercizi.interfaces.comparebile;

public class cerchio extends forna2d implements comparebile {
  double area;

  @Override
  public double area(double raggio) {
    area = raggio * raggio * 3.14;
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

  @Override
  public int compareArea(comparebile altrafigura) {
    if (area < altrafigura) {
      return -1;
    }
    if (area > altrafigura) {
      return 1;
    }
    return 0;
  }

}
