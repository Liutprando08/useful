package com.pro.esercizi;

public class demoCinema {
  public static void main(String[] args) {
    cinema cin = new cinema("le avventue di gigi", 2);
    boolean prenota = cin.isSoldOut();
    boolean sold = cin.prenota(4);
  }

}
