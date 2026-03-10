package com.pro.esercizi.demo;

import com.pro.esercizi.geometria.rettangolo;
import com.pro.esercizi.interfaces.comparebile;
import com.pro.esercizi.geometria.cerchio;
import com.pro.esercizi.geometria.forna2d;

public class demoForma2d {

  public static void main(String[] args) {
    comparebile cerchio = new cerchio();
    comparebile rettangolo = new rettangolo(3, 4);

    double perimetro = cerchio.perimetro(5);
    double area = cerchio.area(3);
    int f1 = cerchio.compareArea(rettangolo);
  }

}
