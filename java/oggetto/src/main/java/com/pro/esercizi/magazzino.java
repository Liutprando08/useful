package com.pro.esercizi;

public class magazzino {

  private String codice;
  private int quantita;
  private double prezzo;

  public void stampaScheda(String codice, int quantita, double prezzo) {
    this.codice = codice;
    this.quantita = quantita;
    this.prezzo = prezzo;
    System.out.println("la quantita e " + quantita + "\nil codice e " + codice + "\nil prezzo e"+prezzo);
  }

  public double totaleValore(int quantita, double prezzo) {

    double totale = prezzo * quantita;
    return totale;
  }
}
