
package com.pro.esercizi;

public class anagrafica {
  private String nome;
  private String cognome;
  private int eta;

  public void completo(String nome, String cognome) {
    this.nome = nome;
    this.cognome = cognome;
    System.out.println(nome + " " + cognome);
  }

  public void ismaggiorenne(int eta) {
    if (eta > eta) {
      System.out.println("e maggiorenne");
    } else {
      System.out.println("e minorenne");
    }
  }
}
