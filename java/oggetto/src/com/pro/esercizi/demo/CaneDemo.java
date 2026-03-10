package com.pro.esercizi.demo;

import com.pro.esercizi.other.cane;

public class CaneDemo {

  public static void main(String[] args) {
    cane pippo = new cane();
    pippo.setEta(12);
    pippo.setNome("gigio");
    pippo.setRazza("arancio");
    pippo.descrizioneCane();
    int etaumani = pippo.etaInAnniUmani(12);
  }
}
