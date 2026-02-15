package com.pro.esercizi;

public class testspedizione {

  public static void main(String[] args) {
    spedizione estero = new internazionale("dakn", 1);
    spedizione veloce = new express("jnscs", 1);
    estero.setZona("kldam");
    veloce.setSabato(true);
    veloce.costo();
    String prova2 = veloce.toString();

    estero.costo();
    String prova1 = estero.toString();
    System.out.println(prova1);
    System.out.println(prova2);
  }
}
