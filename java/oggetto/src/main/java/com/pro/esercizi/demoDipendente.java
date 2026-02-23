
package com.pro.esercizi;

public class demoDipendente {

  public static void main(String[] args) {
    impiegato imp1 = new impiegato("pippo", 1032, 1000, 2);
    tecnico tec1 = new tecnico("gigio", 234, 1500, 3, true);
    UfficioPersonale uff1 = new UfficioPersonale(imp1, tec1);
    double totale = uff1.totaleStipendi();
    System.out.println(totale);
  }
}
