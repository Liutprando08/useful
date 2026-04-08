package com.pro.esercizi.demo;

import com.pro.esercizi.geometria.rettangolo;

public class demoRettangolo {

  public static void main(String[] args) {
    rettangolo geo = new rettangolo(1.0, 2.0);
    geo.setAltezza(2.0);
    geo.setBase(1.0);
    geo.calcolaArea(1.0, 2.0);
    geo.calcolaPerimetro(1.0, 2.0);
    boolean ugu = geo.isQuadrato(2.0, 1.0);
    boolean ugu1 = geo.sonoUguali(1.0, 2.0, 1.0, 2.0);
  }
}
