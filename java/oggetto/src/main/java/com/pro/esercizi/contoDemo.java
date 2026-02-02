package com.pro.esercizi;

public class contoDemo {
  public static void main(String[] args) {
    contoCorrente con1 = new contoCorrente("gigio", 1000);
    contoCorrente con2 = new contoCorrente("giggi", 2000);
    con1.preleva(300);
    con1.deposita(100);
    con2.preleva(1);
    con2.deposita(2);
  }
}
