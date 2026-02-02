package com.pro.esercizi;

public class appartamentoDemo {
  public static void main(String[] args) {
    appartamento app1 = new appartamento("pippo", 14, 1000, 4);
    appartamento app2 = new appartamento("gigio", 16, 43, 5);
    appartamento app3 = new appartamento("liutprando", 20, 200, 2);

    app2.setName("pasquale");
    app3.setInquilini(56789);

    String stampa1 = app1.toString();
    String stampa2 = app2.toString();
    String stampa3 = app3.toString();

    System.out.println(stampa1);
    System.out.println(stampa2);
    System.out.println(stampa3);

  }
}
