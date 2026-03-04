package com.pro.esercizi.demo;

import com.pro.esercizi.geometria.cerchio;
import com.pro.esercizi.geometria.forna2d;

public class demoForma2d {

  public static void main(String[] args) {
    forna2d cerchio = new cerchio();

    double perimetro = cerchio.perimetro(5);
    System.out.println(perimetro);
  }
}
